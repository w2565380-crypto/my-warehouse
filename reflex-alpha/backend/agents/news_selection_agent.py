import json

from services.llm import chat

NEWS_SELECTION_PROMPT = """
You are a financial news ranking agent.

Given a list of news headlines:

1. Select the MOST market-relevant news
2. Prefer:
   - macroeconomic impact
   - large-cap tech
   - AI industry
   - semiconductor industry
   - geopolitical events
   - earnings impact

Return ONLY valid JSON.

Format:

{
  "selected_index": 0,
  "reason": "..."
}
"""

def run_news_selection_agent(news):

    headlines_text = ""

    for i, item in enumerate(news):

        headlines_text += (
            f"{i}. {item['title']}\n"
        )

    response = chat([
        {
            "role": "system",
            "content": NEWS_SELECTION_PROMPT
        },
        {
            "role": "user",
            "content": headlines_text
        }
    ])

    cleaned = (
        response
        .replace("```json", "")
        .replace("```", "")
        .strip()
    )

    return json.loads(cleaned)