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

from graph.reasoning_engine import (
    find_competitors
)

from agents.reasoning_agent import (
    run_reasoning_agent
)

from tools.news_tool import (
    get_latest_news
)

from agents.news_selection_agent import (
    run_news_selection_agent
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
    news = get_latest_news()
    selection = run_news_selection_agent(
        news
    )

    selected_index = selection[
        "selected_index"
    ]

    headline = news[
        selected_index
    ]["title"]

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

    # 在这里添加了指定的硬编码知识图谱关系
    add_relationship(
        "AMD",
        "competes_with",
        "NVIDIA"
    )

    add_relationship(
        "AMD",
        "belongs_to",
        "Semiconductor Sector"
    )

    add_relationship(
        "TSMC",
        "supplies",
        "NVIDIA"
    )

    competitors = find_competitors("AMD")
    graph_context = extracted["relationships"]

    reasoning = run_reasoning_agent(
    headline,
    graph_context
)

    return {
        "headline": headline,
        "initial_analysis": {
            "sentiment": analysis["sentiment"],
            "confidence": analysis["confidence"],
            "reasoning": analysis["reasoning"]
        },
        "reflection": reflection,
        "knowledge_graph": extracted,
        "competitors": competitors,
        "reasoning_agent": reasoning,
        "news": news,
        "news_selection": selection
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