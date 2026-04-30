# state.py
from typing import TypedDict, Optional, Dict, Any

class AgentState(TypedDict, total=False):
    input_task: str
    raw_data: str
    analysis: Dict[str, Any]   # 👈 必须加
    pain_points: str
    revision_count: int