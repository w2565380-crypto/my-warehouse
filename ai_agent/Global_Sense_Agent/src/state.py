# state.py
from typing import TypedDict, List

class AgentState(TypedDict):
    input_task: str
    raw_data: str
    pain_points: str  # 或者使用 List[str]
    revision_count: int
    final_report: str