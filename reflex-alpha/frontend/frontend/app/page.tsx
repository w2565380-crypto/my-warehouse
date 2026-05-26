"use client";

import GraphView from "./components/GraphView";
import { useState } from "react";
import styles from "./Home.module.css";

interface AnalysisResult {
  headline: string;

  initial_analysis: {
    sentiment: string;
    confidence: number;
    reasoning: string;
  };

  reflection: string;

  knowledge_graph: any;

  reasoning_agent: string;
}

export default function Home() {

  const [loading, setLoading] =
    useState<boolean>(false);

  const [result, setResult] =
    useState<AnalysisResult | null>(null);

  const [thinkingSteps, setThinkingSteps] =
    useState<string[]>([]);

  const [graphData, setGraphData] =
  useState<any[]>([]);

  async function runAnalysis() {

    setLoading(true);

    setResult(null);

    setThinkingSteps([]);

    // --------------------------------
    // Start Streaming Agent Thoughts
    // --------------------------------

    const eventSource = new EventSource(
      "http://127.0.0.1:8000/analyze-stream"
    );

    eventSource.onmessage = (
      event: MessageEvent
    ) => {

      setThinkingSteps((prev) => [
        ...prev,
        event.data
      ]);
    };

    // --------------------------------
    // Fetch Final Analysis
    // --------------------------------

    try {

      const response = await fetch(
        "http://127.0.0.1:8000/analyze"
      );

      const data = await response.json();

      const graphResponse = await fetch(
        "http://127.0.0.1:8000/graph"
      );

      const graphJson =
        await graphResponse.json();

      setGraphData(
        graphJson.relationships || []
      );

      setResult(data);

    } catch (error) {

      console.error(error);

    } finally {

      setLoading(false);

      eventSource.close();
    }
  }

  return (

    <main className={styles.page}>

      {/* -------------------------------- */}
      {/* Header */}
      {/* -------------------------------- */}

      <header className={styles.header}>
        <div>
          <h1 className={styles.title}>
            ReflexAlpha
          </h1>
          <p className={styles.subtitle}>
            Graph-Augmented Financial AI Agent
          </p>
        </div>
        <button
          onClick={runAnalysis}
          disabled={loading}
          className={styles.analyzeButton}
        >
          {loading
            ? "Agents Running..."
            : "Run AI Analysis"}
        </button>
      </header>

      {/* -------------------------------- */}
      {/* Dashboard Grid */}
      {/* -------------------------------- */}

      <div className={styles.dashboard}>

        {/* Sidebar: Agent Thinking */}

        <aside className={styles.sidebar}>

          <h2 className={styles.sidebarTitle}>
            Agent Thinking
          </h2>

          {thinkingSteps.map(
            (step, index) => (

              <div
                key={index}
                className={styles.thinkingStep}
              >
                {step}
              </div>
            )
          )}

        </aside>

        {/* Main Content: Results */}

        <div className={styles.content}>

          {result && (
            <>

              <div className={styles.card}>

                <h2 className={styles.cardTitle}>
                  Selected Headline
                </h2>

                <p className={styles.cardText}>
                  {result.headline}
                </p>

              </div>

              <div className={styles.card}>

                <h2 className={styles.cardTitle}>
                  Initial Analysis
                </h2>

                <pre className={styles.pre}>
                  {JSON.stringify(
                    result.initial_analysis,
                    null,
                    2
                  )}
                </pre>

              </div>

              <div className={styles.card}>

                <h2 className={styles.cardTitle}>
                  Reflection
                </h2>

                <pre className={styles.pre}>
                  {result.reflection}
                </pre>

              </div>

              <div className={styles.card}>

                <h2 className={styles.cardTitle}>
                  Knowledge Graph
                </h2>

                <GraphView
                    relationships={graphData}
                  />

              </div>

              <div className={styles.card}>

                <h2 className={styles.cardTitle}>
                  Reasoning Agent
                </h2>

                <pre className={styles.pre}>
                  {result.reasoning_agent}
                </pre>

              </div>

            </>
          )}

        </div>

      </div>

    </main>
  );
}
