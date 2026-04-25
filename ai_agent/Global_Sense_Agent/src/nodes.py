import os
from dotenv import load_dotenv
from langchain_openai import ChatOpenAI
from langchain_core.messages import HumanMessage
from state import AgentState

# 加载 .env 环境变量
load_dotenv()

# 初始化 LLM
llm = ChatOpenAI(
    model='deepseek-chat', 
    openai_api_key=os.getenv("DEEPSEEK_API_KEY"), 
    openai_api_base=os.getenv("DEEPSEEK_API_BASE"),
    max_tokens=1024
)

def scraper_node(state: AgentState):
    """
    抓取节点：目前先返回模拟数据，下一步我们会接入 Crawl4AI
    """
    print("---执行：正在抓取亚马逊商品评论数据---")
    
    # 模拟一段真实的亚马逊用户评论，包含一些噪音
    mock_data = """
    Customer Reviews:
    1. "The noise cancelling is great, but the ear pads are very stiff. My ears hurt after 1 hour."
    2. "Cheap plastic feel. I expect more for $200. Connection drops frequently on my iPhone."
    3. "Battery life is amazing! Lasted me a whole week of commuting."
    4. "Worst customer service. The left earbud stopped working and they won't replace it."
    """
    
    return {"raw_data": mock_data}

def analyze_data_node(state: AgentState):
    """分析节点：调用 LLM 提取业务洞察"""
    print("---执行：正在分析竞品痛点---")
    raw_text = state["raw_data"]
    
    system_prompt = """
    你是一位拥有10年经验的亚马逊（Amazon）高级运营专家。
    你的任务是从非结构化的网页抓取数据中，提取以下核心商业维度：
    1. 用户核心痛点 (Pain Points)：用户最不满意的 3-5 个具体点。
    2. 产品优势 (Advantages)：竞品做得好、值得学习的地方。
    3. 潜在机会 (Opportunities)：基于用户抱怨，我们可以如何改进产品？
    4. 虚假评论预警：识别评论中是否有明显的刷单或机器生成痕迹。

    请以 JSON 格式输出，确保逻辑严密。
    """
    
    prompt = f"{system_prompt}\n\n以下是原始抓取数据：\n{raw_text}"
    
    # 调用大模型
    response = llm.invoke([HumanMessage(content=prompt)])
    
    # 将 AI 的回复存入 state
    return {
        "pain_points": response.content, 
        "revision_count": state.get("revision_count", 0) + 1
    }