import json

from services.llm import chat

ENTITY_EXTRACTION_PROMPT = """
You are a financial knowledge graph extraction agent.

Extract:

1. entities
2. relationships

Allowed relationship types:

- affects
- restricted_by
- belongs_to
- operates_in
- competes_with
- invests_in
- supplies
- partners_with

ONLY use these relationship labels.

ONLY return valid JSON.

Format:

{
  "entities": [
    "..."
  ],

  "relationships": [
    {
      "source": "...",
      "relation": "...",
      "target": "..."
    }
  ]
}
"""

def run_entity_extraction_agent(headline):

    response = chat([
        {
            "role": "system",
            "content": ENTITY_EXTRACTION_PROMPT
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