import os
import asyncio
import sys
import json
from dotenv import load_dotenv
from langchain_openai import ChatOpenAI
from langchain_core.messages import HumanMessage
from state import AgentState
from scraper import fetch_amazon_data

# 加载 .env 环境变量
load_dotenv()

# 初始化 LLM - 针对 DeepSeek 优化
llm = ChatOpenAI(
    model='deepseek-chat', 
    openai_api_key=os.getenv("DEEPSEEK_API_KEY"), 
    openai_api_base=os.getenv("DEEPSEEK_API_BASE"),
    max_tokens=2048,
    temperature=0.1 # 调低随机性，让 JSON 输出更稳定
)

async def scraper_node(state: dict):
    print("\n--- [节点] 启动抓取流程 ---")
    target_url = state.get("input_task")

    markdown_result = await fetch_amazon_data(target_url)

    if not markdown_result or len(markdown_result) < 200:
        return {"raw_data": "抓取失败：内容为空。"}

    return {"raw_data": markdown_result}

def analyze_data_node(state: AgentState):
    """
    分析节点：调用 LLM 将 Markdown 转化为结构化 JSON。
    """
    print("--- [节点] 正在生成深度洞察报告 ---")
    raw_text = state["raw_data"]
    
    # 极简且强制的 System Prompt，确保 DeepSeek 只输出纯 JSON
    system_prompt = """
    你是一位亚马逊高级运营专家。请根据提供的产品数据，输出一份专业的中文商业分析 JSON。
    
    必须且仅输出以下 JSON 格式（严禁包含 ```json 等 Markdown 标签）：
    {
      必须输出 JSON：
        {
            "product_name": "",
            "price_range_analysis": "",
            "pain_points": [],
            "opportunities": []
        }
    }
    """
    
    # 截断输入，防止超过 LLM 上下文限制（取前 8000 字符通常足够）
    prompt = f"{system_prompt}\n\n原始数据预览：\n{raw_text[:8000]}"
    
    try:
        response = llm.invoke([HumanMessage(content=prompt)])
        # 去除 AI 可能带出的 Markdown 代码块修饰
        clean_content = response.content.replace("```json", "").replace("```", "").strip()

        try:
            parsed = json.loads(clean_content)
        except:
            parsed = {"error": clean_content}
        
        print("--- 分析任务执行完毕 ---")
        return {
            "analysis": parsed,
            "revision_count": state.get("revision_count", 0) + 1
        }
    except Exception as e:
        print(f"❌ LLM 分析阶段出错: {e}")
        return {"pain_points": f"分析失败: {str(e)}"}