import json

from services.llm import chat
from agents.prompts import ANALYST_SYSTEM_PROMPT

def run_analyst_agent(headline):

    response = chat([
        {
            "role": "system",
            "content": ANALYST_SYSTEM_PROMPT
        },
        {
            "role": "user",
            "content": headline
        }
    ])

    cleaned = (
        response
        .replace("```json", "")
        .replace("```", "")
        .strip()
    )

    return json.loads(cleaned)