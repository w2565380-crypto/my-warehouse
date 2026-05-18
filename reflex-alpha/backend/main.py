import os
import json
import time

from dotenv import load_dotenv
from agents.entity_extraction_agent import (
    run_entity_extraction_agent
)
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import StreamingResponse

from openai import OpenAI
from agents.analyst_agent import run_analyst_agent
from agents.reflection_agent import run_reflection_agent

from graph.knowledge_graph import (
    add_relationship,
    get_related_entities
)

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

    analysis = run_analyst_agent(headline)

    reflection = run_reflection_agent(
        headline,
        analysis
    )

# --------------------------------
# Entity Extraction Agent
# --------------------------------

    extracted = run_entity_extraction_agent(
        headline
    )

    for relation in extracted["relationships"]:
        add_relationship(
        relation["source"],
        relation["relation"],
        relation["target"]
    )

    return {
        "headline": headline,

        "initial_analysis": {
            "sentiment": analysis["sentiment"],
            "confidence": analysis["confidence"],
            "reasoning": analysis["reasoning"]
        },

        "reflection": reflection,
        "knowledge_graph": extracted
        
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