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

from graph.neo4j_graph import (
    add_typed_relationship,
    get_graph_data
)

from graph.retrieval import (
    retrieve_related_knowledge
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

@app.get("/graph")
def graph():

    return {
        "relationships": get_graph_data()
    }

# ----------------------------------------
# Main Analysis
# ----------------------------------------

@app.get("/analyze")
def analyze():

    # --------------------------------
    # Get Latest News
    # --------------------------------

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

    # --------------------------------
    # Analyst Agent
    # --------------------------------

    analysis = run_analyst_agent(
        headline
    )

    # --------------------------------
    # Reflection Agent
    # --------------------------------

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

    # --------------------------------
    # Build Entity Type Map
    # --------------------------------

    entity_map = {}

    for entity in extracted.get(
        "entities",
        []
    ):

        entity_map[
            entity["name"]
        ] = entity["type"]

    # --------------------------------
    # Store Typed Relationships
    # --------------------------------

    for relation in extracted.get(
        "relationships",
        []
    ):

        add_typed_relationship(

            relation["source"],

            entity_map.get(
                relation["source"],
                "Entity"
            ),

            relation["relation"],

            relation["target"],

            entity_map.get(
                relation["target"],
                "Entity"
            )
        )

    # --------------------------------
    # Add Background Knowledge
    # --------------------------------

    add_typed_relationship(
        "AMD",
        "Company",
        "COMPETES_WITH",
        "NVIDIA",
        "Company"
    )

    add_typed_relationship(
        "TSMC",
        "Company",
        "SUPPLIES",
        "NVIDIA",
        "Company"
    )

    add_typed_relationship(
        "NVIDIA",
        "Company",
        "BELONGS_TO_SECTOR",
        "Semiconductor",
        "Sector"
    )

    # --------------------------------
    # Graph Reasoning
    # --------------------------------

    
    

    # --------------------------------
    # Reasoning Agent
    # --------------------------------


    retrieved_knowledge = []

    for entity in extracted.get(
        "entities",
        []
    ):

        related = retrieve_related_knowledge(
            entity["name"]
        )

        retrieved_knowledge.extend(
            related
        )

    graph_context = retrieved_knowledge

    reasoning = run_reasoning_agent(
        headline,
        graph_context
    )

    # --------------------------------
    # Final Response
    # --------------------------------

    return {

        "headline": headline,

        "initial_analysis": {

            "sentiment": analysis.get(
                "sentiment"
            ),

            "confidence": analysis.get(
                "confidence"
            ),

            "reasoning": analysis.get(
                "reasoning"
            )
        },

        "reflection": reflection,

        "knowledge_graph": extracted,

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