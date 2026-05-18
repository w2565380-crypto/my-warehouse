import os
import json

from dotenv import load_dotenv

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

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

@app.get("/analyze")
def analyze():

    headline = "NVIDIA faces new export restrictions to China"

    # ---------------------------
    # Agent 1: Initial Analysis
    # ---------------------------

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

    # ---------------------------
    # Agent 2: Reflection
    # ---------------------------

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