import json

from services.llm import chat


REASONING_PROMPT = """
You are an advanced financial reasoning agent.

You are given:

1. A financial news headline
2. Historical knowledge retrieved from a financial knowledge graph

Your task:

- infer second-order market impacts
- identify affected companies and sectors
- reason about supply chains
- identify winners and losers
- leverage graph relationships heavily

Focus on deep financial reasoning.

Return concise but insightful analysis.
"""


def run_reasoning_agent(
    headline,
    graph_context
):

    content = f"""
Headline:

{headline}

Retrieved Knowledge Graph Context:

{json.dumps(graph_context, indent=2)}
"""

    response = chat([

        {
            "role": "system",
            "content": REASONING_PROMPT
        },

        {
            "role": "user",
            "content": content
        }

    ])

    return response