# server.py
import os
import sys
from fastapi import FastAPI, HTTPException
from fastapi.responses import HTMLResponse
from fastapi.middleware.cors import CORSMiddleware
from dotenv import load_dotenv

# 1. 强制加载环境变量
load_dotenv()

# 2. 确保能导入 src 下的模块
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

try:
    from main import run_logic
except ImportError as e:
    print(f"❌ 导入失败: {e}")

app = FastAPI()

app.add_middleware(CORSMiddleware, allow_origins=["*"], allow_methods=["*"])

# 自动定位 index.html 的绝对路径
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
HTML_PATH = os.path.join(BASE_DIR, "index.html")

@app.get("/", response_class=HTMLResponse)
async def index():
    if not os.path.exists(HTML_PATH):
        return f"❌ 找不到 index.html，请确保它在: {HTML_PATH}"
    with open(HTML_PATH, encoding="utf-8") as f:
        return f.read()

@app.get("/analyze")
async def analyze(url: str):
    print(f"📥 收到分析请求: {url}")
    try:
        # 执行你的 Agent 逻辑
        result = await run_logic(url)
        return {"status": "success", "data": result}
    except Exception as e:
        # ⚠️ 关键：把具体错误打出来，不要只报 500
        import traceback
        error_detail = traceback.format_exc()
        print(f"❌ 后端报错:\n{error_detail}")
        return {"status": "error", "message": str(e), "detail": error_detail}

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="127.0.0.1", port=8080)