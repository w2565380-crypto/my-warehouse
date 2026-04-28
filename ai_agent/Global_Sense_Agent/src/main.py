# src/main.py
import sys
import os
import json
import asyncio
from datetime import datetime

# 解决路径问题
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

# 强制 Windows 补丁（独立进程运行时必须要有）
if sys.platform == 'win32':
    asyncio.set_event_loop_policy(asyncio.WindowsProactorEventLoopPolicy())

from langgraph.graph import StateGraph, END
from state import AgentState
from nodes import analyze_data_node, scraper_node

def build_graph():
    workflow = StateGraph(AgentState)
    workflow.add_node("scraper", scraper_node)
    workflow.add_node("analyzer", analyze_data_node)
    workflow.set_entry_point("scraper")
    workflow.add_edge("scraper", "analyzer")
    workflow.add_edge("analyzer", END)
    return workflow.compile()

def save_result(data):
    # 获取项目根目录（src 的上一级）
    base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    folder = os.path.join(base_dir, "data")
    os.makedirs(folder, exist_ok=True)

    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    file_path = os.path.join(folder, f"result_{timestamp}.json")

    with open(file_path, "w", encoding="utf-8") as f:
        json.dump(data, f, ensure_ascii=False, indent=2)

    print(f"📁 已保存到: {file_path}")

async def run_logic(url: str):
    app = build_graph()
    initial_state = {
        "input_task": url,
        "revision_count": 0,
        "raw_data": "",
        "pain_points": ""
    }
    # 运行并获取最终状态
    final_result = await app.ainvoke(initial_state)
    return final_result

if __name__ == "__main__":
    # 支持从命令行接收 URL
    target_url = sys.argv[1] if len(sys.argv) > 1 else "http://books.toscrape.com"
    
    try:
        # 使用 asyncio.run 运行，这是最干净的启动方式
        result = asyncio.run(run_logic(target_url))
        save_result(result)
        
        # 将结果以 JSON 格式打印出来，方便 app.py 捕获
        # 注意：这里我们只打印最终的 JSON 字符串
        print("---RESULT_START---")
        print(json.dumps(result, ensure_ascii=False, indent=2))
        print("---RESULT_END---")
        
    except Exception as e:
        print(f"ERROR: {e}", file=sys.stderr)
        sys.exit(1)