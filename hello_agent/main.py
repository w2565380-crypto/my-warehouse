import os
import re
from dotenv import load_dotenv
from tools import available_tools
from agent import OpenAICompatibleClient
from prompt import AGENT_SYSTEM_PROMPT

# 1. 加载配置
load_dotenv()

def main():
    # 2. 初始化 LLM 客户端
    # 注意：确保 .env 文件里有对应的变量名
    client = OpenAICompatibleClient(
        model=os.getenv("MODEL_NAME"),
        api_key=os.getenv("OPENAI_API_KEY"),
        base_url=os.getenv("OPENAI_BASE_URL")
    )

    # 3. 初始化对话状态
    user_prompt = "你好，请帮我查询一下今天深圳的天气，然后根据天气推荐一个合适的旅游景点。"
    prompt_history = [f"用户请求: {user_prompt}"]

    print(f"🚀 用户输入: {user_prompt}")
    print("="*50)

    # 4. 运行 ReAct 主循环
    for i in range(5):  # 限制最大步数，防止死循环
        print(f"\n--- 🔄 循环 {i+1} ---")
        
        # 4.1 拼接上下文并调用大脑
        full_context = "\n".join(prompt_history)
        llm_output = client.generate(full_context, system_prompt=AGENT_SYSTEM_PROMPT)
        
        # 4.2 记录并打印 LLM 的思考
        print(f"🤖 模型输出:\n{llm_output}")
        prompt_history.append(llm_output)

        # 4.3 解析 Action
        action_match = re.search(r"Action[:：]\s*(.*)", llm_output)
        if not action_match:
            obs = "错误：未能解析到 Action，请检查输出格式。"
            prompt_history.append(f"Observation: {obs}")
            continue

        action_str = action_match.group(1).strip()

        # 如果是 Finish，直接结束
        if action_str.startswith("Finish"):
            final_match = re.match(r"Finish\[(.*)\]", action_str)
            final_answer = final_match.group(1) if final_match else action_str
            print(f"\n✅ 任务完成！\n最终答案: {final_answer}")
            break

        # 4.4 执行具体工具
        try:
            # 简单解析函数名和参数 (例如: get_weather(city="北京"))
            tool_name = re.search(r"(\w+)\(", action_str).group(1)
            args_str = re.search(r"\((.*)\)", action_str).group(1)
            # 将字符串参数转为字典
            kwargs = dict(re.findall(r'(\w+)="([^"]*)"', args_str))

            if tool_name in available_tools:
                print(f"🛠️ 正在执行工具: {tool_name} 参数: {kwargs}")
                observation = available_tools[tool_name](**kwargs)
            else:
                observation = f"错误：未定义的工具 '{tool_name}'"
        except Exception as e:
            observation = f"错误：解析或执行工具时出错 - {e}"

        # 4.5 反馈观察结果
        obs_str = f"Observation: {observation}"
        print(f"👁️ 观察结果: {observation}")
        prompt_history.append(obs_str)

if __name__ == "__main__":
    main()