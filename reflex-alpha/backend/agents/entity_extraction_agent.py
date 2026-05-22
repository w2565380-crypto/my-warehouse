import json

from services.llm import chat

from graph.ontology import (
    ALLOWED_RELATIONS
)

ENTITY_EXTRACTION_PROMPT = f"""
Extract:
1. typed entities
2. relationships

ONLY use the following relationship types:
{ALLOWED_RELATIONS}

⚠️ CRITICAL DIRECTION RULES:
- Ensure the direction of relationships is logically and causally correct.
- Macro factors, countries, or events should AFFECT market indices or companies, NOT the other way around. 
  - Good: "US" -> "AFFECTS" -> "Dow"
  - Bad: "Dow" -> "AFFECTS" -> "US"
- Geopolitical restrictions or sanctions flow from the initiator/source to the impacted country.
  - Good: "US" -> "RESTRICTED_BY" -> "Iran" (or vice versa, depending on your ALLOWED_RELATIONS definition. If Iran is limiting the US, source is Iran, target is US).

Return ONLY valid JSON.

Format:
{{
  "entities": [
    {{
      "name": "",
      "type": ""
    }}
  ],
  "relationships": [
    {{
      "source": "",
      "relation": "",
      "target": ""
    }}
  ]
}}

Allowed entity types:
- Company
- Country
- Sector
- Index
- Technology
- Government
- Person
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

    extracted = json.loads(cleaned)

    valid_relationships = []

    for rel in extracted["relationships"]:

        if (
            rel["relation"]
            in ALLOWED_RELATIONS
        ):

            valid_relationships.append(rel)

    extracted["relationships"] = (
        valid_relationships
    )

    return extracted