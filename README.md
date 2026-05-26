# ReflexAlpha

<div align="center">

**多智能体金融推理系统** &nbsp;|&nbsp; Multi-Agent Financial Reasoning Engine

[![Python](https://img.shields.io/badge/Python-3.12-3776AB?logo=python)](https://www.python.org/)
[![FastAPI](https://img.shields.io/badge/FastAPI-0.115-009688?logo=fastapi)](https://fastapi.tiangolo.com/)
[![Next.js](https://img.shields.io/badge/Next.js-16-black?logo=next.js)](https://nextjs.org/)
[![Neo4j](https://img.shields.io/badge/Neo4j-5-4581C3?logo=neo4j)](https://neo4j.com/)
[![DeepSeek](https://img.shields.io/badge/LLM-DeepSeek-4B8BF5)](https://deepseek.com/)
[![ReactFlow](https://img.shields.io/badge/Graph-ReactFlow-FF0072)](https://reactflow.dev/)

</div>

---

## 目录

- [项目简介](#项目简介)
- [核心架构：8 步 Agent 智能体流水线](#核心架构8-步-agent-智能体流水线)
- [大模型集成](#大模型集成)
- [双轨图数据库设计](#双轨图数据库设计)
- [前端渲染：SSE 流式传输 + ReactFlow 动态图谱](#前端渲染sse-流式传输--reactflow-动态图谱)
- [项目结构](#项目结构)
- [快速启动](#快速启动)

---

## 项目简介

ReflexAlpha 是一个**基于 Python / FastAPI 构建的多智能体协作系统**，专为金融领域的深度推理设计。系统的核心思想源于**反思式 AI（Reflexion AI）**——让多个专业化智能体协同工作，通过"生成-反思-推理"的循环链路，从非结构化的财经新闻中提取结构化洞察，并在持续增长的知识图谱上进行二阶推理。

与传统单次问答不同，ReflexAlpha 的每个智能体都是流水线上独立的一环：有的负责信息筛选，有的负责情感分析，有的专门挑逻辑漏洞，有的负责从文本中剥离实体关系并写入图数据库。这种**分而治之**的设计使系统具备了单一大模型难以实现的深度推理能力。

---

## 核心架构：8 步 Agent 智能体流水线

项目的灵魂位于后端 `/analyze` 端点。每次分析请求都会**严格按顺序**触发一条 8 步智能体流水线。每一步的输出都是下一步的输入，形成一条精密的信息加工链条：

```
┌──────────────┐    ┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│ ① 新闻采集    │───▶│ ② 新闻筛选    │───▶│ ③ 分析师      │───▶│ ④ 反思        │
│ News Collector│    │ News Selector│    │ Analyst Agent│    │ Reflection   │
└──────────────┘    └──────────────┘    └──────────────┘    └──────────────┘
                                                                   │
       ┌───────────────────────────────────────────────────────────┘
       ▼
┌──────────────┐    ┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│ ⑤ 实体抽取    │───▶│ ⑥ 图谱写入    │───▶│ ⑦ 图谱检索    │───▶│ ⑧ 推理        │
│ Entity Extr. │    │ Graph Write  │    │ Graph Retriev│    │ Reasoning    │
└──────────────┘    └──────────────┘    └──────────────┘    └──────────────┘
```

### 各 Agent 分工详解

| 步骤 | 智能体 | 源文件 | 职责 |
|:---:|:---|:---|:---|
| ① | **News Collector** | `tools/news_tool.py` | 通过 `feedparser` 从 Google News RSS 抓取 5 条最新财经头条。内置兜底机制——当 RSS 不可用时返回硬编码的 NVIDIA 出口管制新闻 |
| ② | **News Selector** | `agents/news_selection_agent.py` | 由 LLM 驱动，从 5 条新闻中选出最具市场影响力的那一条。偏好顺序：宏观事件 > 大型科技股 > AI/半导体 > 地缘政治 > 财报 |
| ③ | **Analyst Agent** | `agents/analyst_agent.py` | 对选中头条进行情感分析，返回结构化 JSON：`{sentiment, confidence, reasoning}`。温度设为 0 以保证分析的可复现性 |
| ④ | **Reflection Agent** | `agents/reflection_agent.py` | 反思智能体的核心。它重新审视第 ③ 步的分析结论，主动寻找：**逻辑谬误、薄弱假设、遗漏风险**。这是一个"自我纠错"环节 |
| ⑤ | **Entity Extraction** | `agents/entity_extraction_agent.py` | 从新闻标题中提取**类型化实体**（公司/国家/行业/指数/技术/政府/人物）及它们之间的关系。关系类型必须来自本体白名单，否则被过滤。当 LLM 未提取到任何关系时，自动降级为 `RELATED_TO` 兜底边 |
| ⑥ | **Graph Write** | `graph/neo4j_graph.py` | 将第 ⑤ 步提取的实体关系通过 `MERGE` 语句写入 Neo4j（有则复用，无则创建）。同时注入**背景知识**：AMD 竞争 NVIDIA、台积电供应 NVIDIA、NVIDIA 归属半导体行业 |
| ⑦ | **Graph Retrieval** | `graph/retrieval.py` | 以每个实体为锚点，在 Neo4j 中执行**双向匹配查询**（`MATCH (a)-[r]->(b)`），拉取该实体所有的上下游关系。每个实体最多返回 10 条关联，汇总后作为推理上下文 |
| ⑧ | **Reasoning Agent** | `agents/reasoning_agent.py` | 在"新闻事实 + 知识图谱历史上下文"的双重信息源上进行**二阶推理**：推断供应链波及范围、识别赢家与输家、预测次生市场影响 |

> **设计原则**：每个 Agent 职责单一，输入输出均为结构化的中间产物。这种流水线式设计使得任何一个环节都可以独立替换或升级（例如换用更强的模型、更复杂的反思策略），而不会影响上下游。

---

## 大模型集成

### 统一调用层

所有 Agent 共享同一个 LLM 调用入口——`services/llm.py` 中的 `chat()` 函数：

```python
from openai import OpenAI

client = OpenAI(
    api_key=os.getenv("DEEPSEEK_API_KEY"),
    base_url="https://api.deepseek.com"
)

def chat(messages, temperature=0):
    response = client.chat.completions.create(
        model="deepseek-chat",
        messages=messages,
        temperature=temperature
    )
    return response.choices[0].message.content
```

### 设计考量

| 设计决策 | 说明 |
|:---|:---|
| **OpenAI SDK 桥接** | DeepSeek API 与 OpenAI 接口协议完全兼容，因此直接复用 OpenAI Python SDK，仅修改 `base_url` 和 `api_key` 即可无缝切换 |
| **temperature = 0** | 所有 Agent 调用均使用温度 0，确保金融分析结果的可复现性与一致性——相同的输入始终得到相同的输出 |
| **DeepSeek-Chat 满血版** | 底层调用 DeepSeek 旗舰对话模型，具备强大的中英文理解能力与金融领域知识 |
| **集中在 services/llm.py** | 所有 Agent 文件只依赖 `chat()` 一个函数。未来如需切换模型供应商（例如从 DeepSeek 迁至其他兼容 OpenAI 协议的 API），只需修改这一个文件 |

### Agent 如何消费 LLM 能力

每个 Agent 的模式是一致的：

1. 定义一份**系统提示词**（System Prompt），精确描述该 Agent 的角色、输出格式和约束
2. 将"系统提示词 + 用户输入"打包为 messages 数组
3. 调用 `chat()` 获取原始响应
4. 清洗 Markdown 代码块标记（` ```json `），解析为结构化数据
5. 进行业务校验（例如 Entity Extraction Agent 会过滤不在本体白名单中的关系类型）

---

## 双轨图数据库设计

ReflexAlpha 同时维护**两套图存储**，各有分工，互为补充：

### 轨道一：Neo4j（持久化图存储）

```
Neo4j 节点示例：
  (:Company {name: "NVIDIA"})
  (:Company {name: "AMD"})
  (:Company {name: "TSMC"})
  (:Sector  {name: "Semiconductor"})

Neo4j 关系示例：
  (:Company {name: "AMD"})   -[:COMPETES_WITH]-> (:Company {name: "NVIDIA"})
  (:Company {name: "TSMC"})  -[:SUPPLIES]------> (:Company {name: "NVIDIA"})
  (:Company {name: "NVIDIA"})-[:BELONGS_TO_SECTOR]-> (:Sector {name: "Semiconductor"})
```

- **角色**：系统的主要图数据库，负责持久化所有历史分析中提取的实体和关系
- **写入策略**：使用 Cypher `MERGE` 语句实现幂等写入——同一实体/关系不会重复创建
- **查询能力**：支持 Cypher 图模式匹配，可沿任意路径遍历关联实体
- **启动依赖**：需要本地运行 Neo4j 实例（默认 `bolt://localhost:7687`）

### 轨道二：NetworkX（内存图计算）

```python
# graph/knowledge_graph.py
import networkx as nx
graph = nx.Graph()

def find_competitors(company):
    # 在内存图中查找指定公司的所有竞争者
    ...
```

- **角色**：轻量级内存图，用于快速的拓扑分析和算法原型验证
- **使用场景**：`graph/reasoning_engine.py` 中的 `find_competitors()` 基于此实现
- **状态**：当前版本中 memory graph 的查询结果**未被 `/analyze` 端点直接使用**，保留作为未来图算法扩展的基础设施

### 本体白名单（Ontology Allowlist）

所有写入图谱的关系必须通过 `graph/ontology.py` 中定义的白名单校验：

```python
ALLOWED_RELATIONS = [
    "COMPETES_WITH",     # 竞争关系：AMD → NVIDIA
    "SUPPLIES",          # 供应关系：TSMC → NVIDIA
    "PARTNERS_WITH",     # 合作关系
    "RESTRICTED_BY",     # 受限于：国家A ← 出口管制 ← 国家B
    "BELONGS_TO_SECTOR", # 行业归属：NVIDIA → 半导体
    "AFFECTS",           # 影响关系：政策 → 指数
    "INVESTS_IN",        # 投资关系
    "ACQUIRES",          # 收购关系
    "MANUFACTURES",      # 制造关系
    "USES",              # 使用关系
    "REGULATED_BY"       # 受监管于
]
```

这 11 种关系类型构成了 ReflexAlpha 的**金融本体论**——它定义了系统"理解"世界的方式。Entity Extraction Agent 在写入 Neo4j 前会过滤掉所有不在白名单内的关系，确保图谱的语义一致性。

---

## 前端渲染：SSE 流式传输 + ReactFlow 动态图谱

### SSE（Server-Sent Events）流式推理

前端在用户点击"Run AI Analysis"后，**同时**发起两条请求：

| 请求 | 端点 | 用途 |
|:---|:---|:---|
| `EventSource` | `/analyze-stream` | 以 SSE 格式接收 Agent 各阶段的状态描述（"Collector Agent: Gathering market news..." → "Market Analyst Agent: Evaluating NVIDIA exposure..." → ...），实时渲染在"Agent Thinking"面板中 |
| `fetch` | `/analyze` + `/graph` | 等待流水线完成，获取结构化的最终分析结果和图谱数据 |

```
时间轴 ──────────────────────────────────────────────────────▶

SSE 流   ┌─ Collector ─┬─ Analyst ─┬─ Reflection ─┬─ ... ─┐
         │  Gathering  │  Eval...  │  Checking... │        │
         └─────────────┴───────────┴──────────────┴────────┘

HTTP     ┌────────────────────── 8-step pipeline ──────────┐
         │                                                  │
         └─────────────────── then ────────────────────────┘
                                    │
                                    ▼
                            Final JSON Result
```

### ReactFlow 知识图谱可视化

`app/components/GraphView.tsx` 负责将 Neo4j 返回的关系数组转化为可交互的力导向图：

- **节点**：每个唯一实体生成一个 ReactFlow 节点，带深色卡片样式（`#111827` 背景 + 圆角边框）
- **边**：每条关系生成一条动画边，标签显示关系类型（`COMPETES_WITH`、`SUPPLIES` 等）
- **交互**：内置 Minimap（缩略图导航）、Controls（缩放/居中控制）、Background（网格背景）
- **布局**：基于网格的简单初始布局（4 列），配合 `fitView` 自动适配视口

### 技术栈

| 层 | 技术 | 版本 |
|:---|:---|:---|
| 框架 | Next.js (App Router) | 16.2 |
| UI 库 | React | 19.2 |
| 语言 | TypeScript | 5.x |
| 样式 | Tailwind CSS | 4.x |
| 图可视化 | ReactFlow | 11.11 |
| 字体 | Geist Sans + Geist Mono | — |

---

## 项目结构

```
reflex-alpha/
├── backend/
│   ├── main.py                  # FastAPI 入口，定义 /analyze 等端点
│   ├── .env                     # API Key 与数据库连接配置
│   ├── requirements.txt         # Python 依赖清单
│   ├── agents/
│   │   ├── prompts.py           # 分析师 & 反思 Agent 的系统提示词
│   │   ├── analyst_agent.py     # ③ 金融情感分析 Agent
│   │   ├── reflection_agent.py  # ④ 反思/纠错 Agent
│   │   ├── entity_extraction_agent.py  # ⑤ 实体与关系抽取 Agent
│   │   ├── news_selection_agent.py     # ② 新闻筛选 Agent
│   │   └── reasoning_agent.py   # ⑧ 图谱推理 Agent
│   ├── services/
│   │   └── llm.py               # DeepSeek 大模型统一调用层
│   ├── tools/
│   │   └── news_tool.py         # ① Google News RSS 新闻采集
│   ├── graph/
│   │   ├── neo4j_graph.py       # ⑥ 持久化图谱（Neo4j 读写）
│   │   ├── knowledge_graph.py   # 内存图谱（NetworkX）
│   │   ├── reasoning_engine.py  # 图谱推理算法（竞争者查找等）
│   │   ├── retrieval.py         # ⑦ 图谱知识检索
│   │   └── ontology.py          # 本体白名单（11 种关系类型）
│   ├── models/                  # 数据模型（预留）
│   └── routes/                  # 路由拆分（预留）
│
├── frontend/frontend/
│   ├── app/
│   │   ├── layout.tsx           # 根布局（Geist 字体）
│   │   ├── page.tsx             # 主页：SSE 流 + 分析结果渲染
│   │   ├── globals.css          # Tailwind 全局样式
│   │   └── components/
│   │       └── GraphView.tsx    # ReactFlow 知识图谱可视化
│   ├── package.json
│   ├── next.config.ts
│   └── tsconfig.json
│
└── CLAUDE.md                    # Claude Code 协助开发指南
```

---

## 快速启动

### 前置条件

- **Python 3.12+** & `pip`
- **Node.js 20+** & `npm`
- **Neo4j 数据库**运行于 `localhost:7687`（[下载 Neo4j Desktop](https://neo4j.com/download/) 或使用 Docker）
- **DeepSeek API Key**（[获取地址](https://platform.deepseek.com/)）

### 1. 配置环境变量

编辑 `backend/.env`，填入你的 API Key 和 Neo4j 凭据：

```env
DEEPSEEK_API_KEY=sk-your-key-here
NEO4J_URI=bolt://localhost:7687
NEO4J_USERNAME=neo4j
NEO4J_PASSWORD=your-password
```

### 2. 启动后端

```bash
cd backend

# 创建虚拟环境（首次）
python -m venv venv

# 激活虚拟环境
# Windows:
venv\Scripts\activate
# macOS / Linux:
source venv/bin/activate

# 安装依赖
pip install fastapi uvicorn openai python-dotenv neo4j feedparser networkx

# 启动服务
uvicorn main:app --reload --port 8000
```

启动后访问 [http://127.0.0.1:8000](http://127.0.0.1:8000) 确认后端运行正常。

### 3. 启动前端

```bash
cd frontend/frontend

# 安装依赖（首次）
npm install

# 启动开发服务器
npm run dev
```

启动后访问 [http://localhost:3000](http://localhost:3000)，点击 **"Run AI Analysis"** 即可触发完整的 8 步智能体流水线。

### 4. 验证全链路

1. 点击 "Run AI Analysis" 按钮
2. 观察左侧 "Agent Thinking" 面板实时显示各 Agent 的执行状态
3. 等待流水线完成后，页面依次展示：
   - **Selected Headline** — 被选中的财经头条
   - **Initial Analysis** — 分析师的情感判断与置信度
   - **Reflection** — 反思 Agent 的纠错意见
   - **Knowledge Graph** — ReactFlow 渲染的交互式图谱
   - **Reasoning Agent** — 基于图谱的二阶推理结论

---

<div align="center">

**ReflexAlpha** — *让智能体学会反思，让图谱承载推理*

</div>
