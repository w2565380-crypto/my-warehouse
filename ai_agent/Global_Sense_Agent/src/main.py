# main.py
from langgraph.graph import StateGraph, END
from state import AgentState
from nodes import analyze_data_node, scraper_node

# 1. 初始化图
workflow = StateGraph(AgentState)

# 2. 添加节点
workflow.add_node("scraper", scraper_node)
workflow.add_node("analyzer", analyze_data_node)

# 3. 设置连线
workflow.set_entry_point("scraper")
workflow.add_edge("scraper", "analyzer")
workflow.add_edge("analyzer", END)

# 4. 编译并运行
app = workflow.compile()

if __name__ == "__main__":
    inputs = {"input_task": "分析亚马逊某款耳机"}
    for output in app.stream(inputs):
        print(output)