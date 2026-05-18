import os
import json
import time

from dotenv import load_dotenv

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import StreamingResponse

from openai import OpenAI

load_dotenv()

client = OpenAI(
    api_key=os.getenv("DEEPSEEK_API_KEY"),
    base_url="https://api.deepseek.com"
)

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# ----------------------------------------
# Root
# ----------------------------------------

@app.get("/")
def root():

    return {
        "message": "ReflexAlpha Backend Running"
    }

# ----------------------------------------
# Main Analysis
# ----------------------------------------

@app.get("/analyze")
def analyze():

    headline = "NVIDIA faces new export restrictions to China"

    # -----------------------------
    # Agent 1: Initial Analysis
    # -----------------------------

    analysis_response = client.chat.completions.create(
        model="deepseek-chat",
        messages=[
            {
                "role": "system",
                "content": """
You are an elite financial analyst.

ONLY return valid JSON.

Format:
{
  "sentiment": "...",
  "confidence": 0.0,
  "reasoning": "..."
}
"""
            },
            {
                "role": "user",
                "content": headline
            }
        ],
        temperature=0
    )

    analysis_raw = (
        analysis_response
        .choices[0]
        .message
        .content
    )

    analysis_cleaned = (
        analysis_raw
        .replace("```json", "")
        .replace("```", "")
        .strip()
    )

    analysis = json.loads(analysis_cleaned)

    # -----------------------------
    # Agent 2: Reflection
    # -----------------------------

    reflection_response = client.chat.completions.create(
        model="deepseek-chat",
        messages=[
            {
                "role": "system",
                "content": """
You are a critique agent.

Review the financial analysis.

Find:
- logical flaws
- weak assumptions
- missing risks

Be concise.
"""
            },
            {
                "role": "user",
                "content": f"""
Headline:
{headline}

Analysis:
{analysis}
"""
            }
        ],
        temperature=0
    )

    reflection = (
        reflection_response
        .choices[0]
        .message
        .content
    )

    return {
        "headline": headline,

        "initial_analysis": {
            "sentiment": analysis["sentiment"],
            "confidence": analysis["confidence"],
            "reasoning": analysis["reasoning"]
        },

        "reflection": reflection
    }

# ----------------------------------------
# Streaming Agent Thinking
# ----------------------------------------

@app.get("/analyze-stream")
def analyze_stream():

    def event_generator():

        events = [
            "Collector Agent: Gathering market news...",
            "Collector Agent: Found semiconductor-related articles...",
            "Market Analyst Agent: Evaluating NVIDIA exposure...",
            "Market Analyst Agent: China revenue risk detected...",
            "Reflection Agent: Checking for overconfidence...",
            "Reflection Agent: Revising confidence score...",
            "Final Conclusion: Short-term bearish sentiment."
        ]

        for event in events:

            yield f"data: {event}\n\n"

            time.sleep(1)

    return StreamingResponse(
        event_generator(),
        media_type="text/event-stream"
    )