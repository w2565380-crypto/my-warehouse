# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ReflexAlpha is a multi-agent financial reasoning system. The backend orchestrates a pipeline of LLM-powered agents (news selection, analysis, reflection, entity extraction, reasoning) that process financial news headlines through a Neo4j-backed knowledge graph. The frontend displays results via SSE streaming and ReactFlow graph visualization.

## Commands

### Backend (Python/FastAPI)

```bash
cd backend
# Activate venv: venv/Scripts/activate (Windows) or source venv/bin/activate (Unix)
pip install fastapi uvicorn openai python-dotenv neo4j feedparser networkx
uvicorn main:app --reload --port 8000
```

### Frontend (Next.js)

```bash
cd frontend/frontend
npm install
npm run dev        # Start dev server
npm run build      # Production build
npm run lint       # ESLint
```

No test suite currently exists. The `backend/test_neo4j.py` file is a one-off smoke test script, not part of a test framework.

## Architecture

### Backend Pipeline (`backend/main.py`)

The `/analyze` endpoint runs a sequential multi-agent pipeline:

1. **News Collection** (`tools/news_tool.py`) — Fetches top 5 headlines from Google News RSS via `feedparser`. Falls back to a hardcoded NVIDIA headline if RSS is empty.
2. **News Selection** (`agents/news_selection_agent.py`) — LLM ranks headlines by market relevance, returns `selected_index`.
3. **Analyst** (`agents/analyst_agent.py`) — Returns JSON: `{sentiment, confidence, reasoning}`.
4. **Reflection** (`agents/reflection_agent.py`) — Critiques the analysis for logical flaws, weak assumptions, missing risks.
5. **Entity Extraction** (`agents/entity_extraction_agent.py`) — Extracts typed entities and relationships from the headline. Validates relationships against `graph/ontology.py` allowlist. Falls back to a generic `RELATED_TO` edge if no valid relationships found.
6. **Graph Storage** (`graph/neo4j_graph.py`) — Persists typed relationships to Neo4j with `MERGE` (upsert). Also injects hardcoded background knowledge (AMD competes with NVIDIA, TSMC supplies NVIDIA, NVIDIA in Semiconductor sector).
7. **Graph Retrieval** (`graph/retrieval.py`) — For each extracted entity, queries Neo4j for related nodes/edges (bidirectional match, limit 10).
8. **Reasoning** (`agents/reasoning_agent.py`) — Uses retrieved graph context to infer second-order market impacts, supply chain effects, winners/losers.

The `/analyze-stream` endpoint returns hardcoded SSE events for UI demo purposes (not connected to the actual agent pipeline).

### LLM Integration (`backend/services/llm.py`)

All agents call DeepSeek's API (`deepseek-chat` model) via the OpenAI SDK (base URL `https://api.deepseek.com`). The `chat()` function wraps `client.chat.completions.create` with temperature 0. Configuration is in `backend/.env`.

### Knowledge Graph (`backend/graph/`)

Two parallel graph implementations coexist:

- **`neo4j_graph.py`** — Persistent typed graph. Nodes have labels (Company, Country, Sector, etc.) and a `name` property. Edges use relationship types from `ontology.py` allowlist. Used by the API in production.
- **`knowledge_graph.py`** — In-memory NetworkX graph. Only used by `reasoning_engine.py` for `find_competitors()`, which is imported but not called in `main.py`.

Ontology (`graph/ontology.py`) defines 11 allowed relationship types: `COMPETES_WITH`, `SUPPLIES`, `PARTNERS_WITH`, `RESTRICTED_BY`, `BELONGS_TO_SECTOR`, `AFFECTS`, `INVESTS_IN`, `ACQUIRES`, `MANUFACTURES`, `USES`, `REGULATED_BY`.

### Frontend (`frontend/frontend/`)

- **Next.js 16 / React 19 / TypeScript / Tailwind CSS 4**
- **`app/page.tsx`** — Single-page app. On "Run AI Analysis" click: opens SSE connection to `/analyze-stream` for live thinking steps, then fetches `/analyze` and `/graph` for final results.
- **`app/components/GraphView.tsx`** — Renders graph data as a ReactFlow interactive graph with nodes, animated edges with labels, minimap, and controls.
- **`app/layout.tsx`** — Root layout with Geist font.
- Hardcoded backend URL `http://127.0.0.1:8000` in `page.tsx`.

### Key Architectural Notes

- There is no database ORM or migration system. Neo4j schema is created implicitly via `MERGE` queries.
- The `models/`, `routes/`, and `services/__init__.py` directories exist but are empty — they are placeholders for planned expansion.
- The frontend's `react-force-graph` and `react-force-graph-2d` dependencies are installed but unused; only `reactflow` is used for graph visualization.
- Agent prompts are split between `agents/prompts.py` (analyst/reflection) and inline constants in each agent file.
- The project uses Python 3 (with venv in `backend/venv/`) and requires a local Neo4j instance at `bolt://localhost:7687`.
