"use client";

import GraphView from "./components/GraphView";
import { useState } from "react";

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

    <main
      style={{
        padding: "40px",
        fontFamily: "Arial",
        backgroundColor: "#0f172a",
        minHeight: "100vh",
        color: "white"
      }}
    >

      <h1
        style={{
          fontSize: "42px",
          marginBottom: "20px"
        }}
      >
        ReflexAlpha
      </h1>

      <button
        onClick={runAnalysis}
        disabled={loading}
        style={{
          padding: "12px 20px",
          fontSize: "16px",
          cursor: "pointer",
          marginBottom: "30px",
          borderRadius: "8px",
          border: "none",
          backgroundColor: "#2563eb",
          color: "white"
        }}
      >
        {loading
          ? "Agents Running..."
          : "Run AI Analysis"}
      </button>

      {/* -------------------------------- */}
      {/* Agent Thinking Stream */}
      {/* -------------------------------- */}

      <div
        style={{
          background: "#111827",
          color: "#00ff99",
          padding: "20px",
          borderRadius: "10px",
          marginBottom: "30px",
          minHeight: "220px",
          fontFamily: "monospace",
          border: "1px solid #1f2937"
        }}
      >

        <h2
          style={{
            marginBottom: "20px"
          }}
        >
          Agent Thinking
        </h2>

        {thinkingSteps.map(
          (step, index) => (

            <div
              key={index}
              style={{
                marginBottom: "12px"
              }}
            >
              {step}
            </div>
          )
        )}

      </div>

      {/* -------------------------------- */}
      {/* Final Result */}
      {/* -------------------------------- */}

      {result && (

        <div>

          <section
            style={{
              marginBottom: "30px"
            }}
          >

            <h2>
              Selected Headline
            </h2>

            <p>
              {result.headline}
            </p>

          </section>

          <section
            style={{
              marginBottom: "30px"
            }}
          >

            <h2>
              Initial Analysis
            </h2>

            <pre
              style={{
                whiteSpace: "pre-wrap"
              }}
            >
              {JSON.stringify(
                result.initial_analysis,
                null,
                2
              )}
            </pre>

          </section>

          <section
            style={{
              marginBottom: "30px"
            }}
          >

            <h2>
              Reflection
            </h2>

            <pre
              style={{
                whiteSpace: "pre-wrap"
              }}
            >
              {result.reflection}
            </pre>

          </section>

          <section
            style={{
              marginBottom: "30px"
            }}
          >

            <h2>
              Knowledge Graph
            </h2>

            <GraphView
                relationships={graphData}
              />

          </section>

          <section
            style={{
              marginBottom: "30px"
            }}
          >

            <h2>
              Reasoning Agent
            </h2>

            <pre
              style={{
                whiteSpace: "pre-wrap"
              }}
            >
              {result.reasoning_agent}
            </pre>

          </section>

        </div>
      )}

    </main>
  );
}