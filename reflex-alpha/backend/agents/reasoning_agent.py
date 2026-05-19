from services.llm import chat

REASONING_PROMPT = """
You are a financial reasoning agent.

Given:
1. market news
2. graph relationships

Infer:

- possible beneficiaries
- secondary market impacts
- sector implications

Be concise.
"""

def run_reasoning_agent(
    headline,
    graph_context
):

    response = chat([
        {
            "role": "system",
            "content": REASONING_PROMPT
        },
        {
            "role": "user",
            "content": f"""
Headline:
{headline}

Graph Context:
{graph_context}
"""
        }
    ])

    return response