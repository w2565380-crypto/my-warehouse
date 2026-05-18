from services.llm import chat
from agents.prompts import REFLECTION_SYSTEM_PROMPT

def run_reflection_agent(headline, analysis):

    response = chat([
        {
            "role": "system",
            "content": REFLECTION_SYSTEM_PROMPT
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
    ])

    return response