# src/app.py
import streamlit as st
import subprocess
import json
import sys

st.set_page_config(page_title="Global Sense Agent", layout="wide")
st.title("🌍 Global Sense Agent")

url_input = st.text_input("请输入 Amazon 商品链接:", value="https://www.amazon.sg/dp/B0F3HFQ5SY")
analyze_btn = st.button("开始分析", type="primary")

if analyze_btn:
    with st.spinner("🚀 正在启动独立分析进程 (隔离模式)..."):
        # 使用 subprocess 调用当前的 Python 解释器运行 main.py
        # 这样 main.py 就在一个全新的进程里，拥有自己完美的事件循环
        try:
            process = subprocess.Popen(
                [sys.executable, "src/main.py", url_input],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                encoding='utf-8'
            )
            
            stdout, stderr = process.communicate()
            
            if process.returncode == 0:
                # 解析捕获到的结果
                if "---RESULT_START---" in stdout:
                    raw_json = stdout.split("---RESULT_START---")[1].split("---RESULT_END---")[0].strip()
                    final_state = json.loads(raw_json)
                    
                    st.success("分析成功！")
                    st.json(final_state["pain_points"]) # 先简单展示，跑通后再美化
                else:
                    st.error("未能获取分析结果，请检查控制台。")
                    st.text_area("详细日志:", stdout)
            else:
                st.error(f"分析进程崩溃: {stderr}")
                
        except Exception as e:
            st.error(f"调用失败: {e}")