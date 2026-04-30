import os
import json
from dotenv import load_dotenv
from langchain_openai import ChatOpenAI
from langchain_core.messages import HumanMessage
from state import AgentState
from scraper import fetch_amazon_data

# 加载环境变量
load_dotenv()

# 初始化 LLM
llm = ChatOpenAI(
    model='deepseek-v4-flash',
    openai_api_key=os.getenv("DEEPSEEK_API_KEY"),
    openai_api_base=os.getenv("DEEPSEEK_API_BASE"),
    max_tokens=2048,
    temperature=0.1
)

# -------------------------------
# 🕷 爬虫节点
# -------------------------------
async def scraper_node(state: dict):
    print("\n--- [节点] 启动抓取流程 ---")
    target_url = state.get("input_task")

    markdown_result = await fetch_amazon_data(target_url)

    if not markdown_result or len(markdown_result) < 200:
        return {"raw_data": "抓取失败：内容为空。"}

    return {"raw_data": markdown_result}


# -------------------------------
# 🧠 分析节点
# -------------------------------
def analyze_data_node(state: AgentState):
    print("--- [节点] 正在生成深度洞察报告 ---")
    raw_text = state["raw_data"]

    system_prompt = """
你是一位电商高级运营专家。请根据提供的产品数据，输出一份中文商业分析。

必须严格输出 JSON（不能有 ``` 或解释）：

{
    "product_name": "",
    "price_range_analysis": "",
    "pain_points": [],
    "opportunities": []
}
"""

    prompt = f"{system_prompt}\n\n原始数据：\n{raw_text[:8000]}"

    try:
        response = llm.invoke([HumanMessage(content=prompt)])

        # 👉 打印原始输出（调试用）
        print("LLM原始输出:", response.content)

        # 清理 markdown 包裹
        clean_content = response.content.replace("```json", "").replace("```", "").strip()

        # 👉 转 JSON
        parsed = json.loads(clean_content)

        print("--- 分析任务执行完毕 ---")

        return {
            "analysis": parsed,
            "revision_count": state.get("revision_count", 0) + 1
        }

    except Exception as e:
        print(f"❌ LLM 分析阶段出错: {e}")

        return {
            "analysis": None,
            "error": str(e),
            "revision_count": state.get("revision_count", 0) + 1
        }