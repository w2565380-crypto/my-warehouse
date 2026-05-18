import os
from tools import ToolExecutor
from ReAct import REACT_PROMPT_TEMPLATE
from openai import OpenAI
from dotenv import load_dotenv
from typing import List, Dict

# 加载 .env 文件中的环境变量
load_dotenv()

class HelloAgentsLLM:
    """
    为本书 "Hello Agents" 定制的LLM客户端。
    它用于调用任何兼容OpenAI接口的服务，并默认使用流式响应。
    """
    def __init__(self, model: str = None, apiKey: str = None, baseUrl: str = None, timeout: int = None):
        """
        初始化客户端。优先使用传入参数，如果未提供，则从环境变量加载。
        """
        self.model = model or os.getenv("LLM_MODEL_ID")
        apiKey = apiKey or os.getenv("LLM_API_KEY")
        baseUrl = baseUrl or os.getenv("LLM_BASE_URL")
        timeout = timeout or int(os.getenv("LLM_TIMEOUT", 60))
        
        if not all([self.model, apiKey, baseUrl]):
            raise ValueError("模型ID、API密钥和服务地址必须被提供或在.env文件中定义。")

        self.client = OpenAI(api_key=apiKey, base_url=baseUrl, timeout=timeout)

    def think(self, messages: List[Dict[str, str]], temperature: float = 0) -> str:
        """
        调用大语言模型进行思考，并返回其响应。
        """
        print(f"🧠 正在调用 {self.model} 模型...")
        try:
            response = self.client.chat.completions.create(
                model=self.model,
                messages=messages,
                temperature=temperature,
                stream=True,
            )
            
            # 处理流式响应
            print("✅ 大语言模型响应成功:")
            collected_content = []
            for chunk in response:
                if not chunk.choices:
                    continue
                content = chunk.choices[0].delta.content or ""
                print(content, end="", flush=True)
                collected_content.append(content)
            print()  # 在流式输出结束后换行
            return "".join(collected_content)

        except Exception as e:
            print(f"❌ 调用LLM API时发生错误: {e}")
            return None
        
import re

class ReActAgent:
    def __init__(self, llm_client, tool_executor, max_steps: int = 5):
        self.llm_client = llm_client
        self.tool_executor = tool_executor
        self.max_steps = max_steps
        self.history = []

    def run(self, question: str):
        """
        运行ReAct智能体来回答一个问题。
        """
        self.history = []  # 每次运行时重置历史记录
        current_step = 0

        while current_step < self.max_steps:
            current_step += 1
            print(f"\n--- 第 {current_step} 步 ---")

            # 1. 格式化提示词
            tools_desc = self.tool_executor.getAvailableTools()
            history_str = "\n".join(self.history)
            prompt = REACT_PROMPT_TEMPLATE.format(
                tools=tools_desc,
                question=question,
                history=history_str
            )

            # 2. 调用LLM进行思考
            messages = [{"role": "user", "content": prompt}]
            response_text = self.llm_client.think(messages=messages)
            
            if not response_text:
                print("错误:LLM未能返回有效响应。")
                break

            print(f"🤖 Agent 思考与行动:\n{response_text}")
            
            # 【重要】将大模型的思考和行动记录到历史记忆中
            self.history.append(response_text)

            # 3. 解析 Action 字段
            action_match = re.search(r"Action:\s*(.*)", response_text)
            if not action_match:
                obs = "错误: 您的回复中未找到 'Action:' 字段。请严格遵循格式要求。"
                print(f"👁️ 观察结果: {obs}")
                self.history.append(f"Observation: {obs}")
                continue

            action_str = action_match.group(1).strip()

            # 3.1 检查是否完成任务 (Finish)
            if action_str.startswith("Finish"):
                final_answer_match = re.match(r"Finish\[(.*)\]", action_str)
                if final_answer_match:
                    final_answer = final_answer_match.group(1)
                else:
                    final_answer = action_str  # 降级处理
                print(f"\n✅ 任务最终完成！\n最终答案: {final_answer}")
                return final_answer

            # 3.2 解析工具名称与参数 (例如: get_weather(city="北京"))
            try:
                tool_name_match = re.search(r"(\w+)\(", action_str)
                args_match = re.search(r"\((.*)\)", action_str)
                
                if not tool_name_match or not args_match:
                    raise ValueError("Action 语法错误，无法解析函数名或参数。")
                
                tool_name = tool_name_match.group(1)
                args_str = args_match.group(1)
                
                # 使用正则将 city="北京", weather="晴天" 这种字符串切分成字典
                kwargs = dict(re.findall(r'(\w+)=\"([^\"]*)\"', args_str))
                
            except Exception as e:
                obs = f"错误: 解析 Action 参数失败 - {e}。请检查参数是否带有双引号，如 tool_name(arg=\"value\")"
                print(f"👁️ 观察结果: {obs}")
                self.history.append(f"Observation: {obs}")
                continue

            # 4. 执行行动并获取观察结果
            print(f"🛠️ 正在调用工具 [{tool_name}]，参数: {kwargs}")
            try:
                # 调用传入的工具执行器
                observation = self.tool_executor.execute(tool_name, **kwargs)
            except Exception as e:
                observation = f"错误: 执行工具时发生系统异常 - {e}"

            # 5. 记录并反馈观察结果
            obs_str = f"Observation: {observation}"
            print(f"👁️ 观察结果:\n{observation}")
            
            # 【重要】将外部世界的反馈塞回记忆，供下一步循环的 LLM 参考
            self.history.append(obs_str)

        print("\n❌ 达到最大步数限制，任务未完成。")
        return "抱歉，我尽力了，但在规定步数内没能找到答案。"


# --- 客户端使用示例 ---
if __name__ == '__main__':
    try:
        llmClient = HelloAgentsLLM()
        
        exampleMessages = [
            {"role": "system", "content": "You are a helpful assistant that writes Python code."},
            {"role": "user", "content": "英伟达最新的GPU型号是什么"}
        ]
        
        print("--- 调用LLM ---")
        responseText = llmClient.think(exampleMessages)
        if responseText:
            print("\n\n--- 完整模型响应 ---")
            print(responseText)

    except ValueError as e:
        print(e)


