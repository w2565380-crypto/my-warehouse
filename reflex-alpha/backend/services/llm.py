import os

from dotenv import load_dotenv
from openai import OpenAI

load_dotenv()

client = OpenAI(
    api_key=os.getenv("DEEPSEEK_API_KEY"),
    base_url="https://api.deepseek.com"
)

def chat(messages, temperature=0):

    response = client.chat.completions.create(
        model="deepseek-chat",
        messages=messages,
        temperature=temperature
    )

    return response.choices[0].message.content