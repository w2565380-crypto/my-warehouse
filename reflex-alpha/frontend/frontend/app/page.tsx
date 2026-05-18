"use client";

import { useEffect, useState } from "react";

export default function Home() {

  const [data, setData] = useState<any>(null);

  const [events, setEvents] = useState<string[]>([]);

  // -----------------------------
  // Fetch analysis result
  // -----------------------------

  useEffect(() => {

    fetch("http://127.0.0.1:8000/analyze")
      .then((res) => res.json())
      .then((data) => {
        setData(data);
      });

  }, []);

  // -----------------------------
  // Streaming Agent Thinking
  // -----------------------------

  useEffect(() => {

    const eventSource = new EventSource(
      "http://127.0.0.1:8000/analyze-stream"
    );

    eventSource.onmessage = (event) => {

      setEvents((prev) => [
        ...prev,
        event.data
      ]);

    };

    return () => {
      eventSource.close();
    };

  }, []);

  return (
    <main className="min-h-screen bg-black text-white p-6">

      {/* Header */}

      <div className="mb-10">

        <h1 className="text-4xl font-bold">
          ReflexAlpha
        </h1>

        <p className="mt-2 text-zinc-400">
          Reflective Multi-Agent Financial Intelligence Platform
        </p>

      </div>

      {/* Loading */}

      {!data ? (

        <p>Loading AI analysis...</p>

      ) : (

        <div className="grid grid-cols-12 gap-4">

          {/* LEFT PANEL */}

          <div className="col-span-3 rounded-2xl border border-zinc-800 p-4">

            <h2 className="mb-4 text-xl font-semibold">
              News Feed
            </h2>

            <div className="rounded-xl bg-zinc-900 p-4">

              <p className="text-sm text-zinc-400 mb-2">
                Reuters
              </p>

              <p>
                {data.headline}
              </p>

            </div>

          </div>

          {/* CENTER PANEL */}

          <div className="col-span-6 rounded-2xl border border-zinc-800 p-4">

            <h2 className="mb-4 text-xl font-semibold">
              AI Analysis
            </h2>

            {/* Initial Analysis */}

            <div className="rounded-xl bg-zinc-900 p-4 mb-4">

              <p className="text-blue-400 font-medium">
                Market Analyst Agent
              </p>

              <p className="mt-3 text-zinc-300">
                {data.initial_analysis.reasoning}
              </p>

            </div>

            {/* Reflection */}

            <div className="rounded-xl bg-zinc-900 p-4">

              <p className="text-yellow-400 font-medium">
                Reflection Agent
              </p>

              <p className="mt-3 text-zinc-300">
                {data.reflection}
              </p>

            </div>

            {/* Streaming Thinking */}

            <div className="mt-8">

              <h2 className="mb-4 text-xl font-semibold">
                Live Agent Thinking
              </h2>

              <div className="rounded-2xl border border-zinc-800 bg-zinc-950 p-4">

                <div className="space-y-3">

                  {events.map((event, index) => (

                    <div
                      key={index}
                      className="rounded-xl bg-zinc-900 p-3 text-sm text-zinc-300"
                    >
                      {event}
                    </div>

                  ))}

                </div>

              </div>

            </div>

          </div>

          {/* RIGHT PANEL */}

          <div className="col-span-3 rounded-2xl border border-zinc-800 p-4">

            <h2 className="mb-4 text-xl font-semibold">
              Market Impact
            </h2>

            <div className="space-y-4">

              {/* Sentiment */}

              <div className="rounded-xl bg-zinc-900 p-4">

                <p className="text-zinc-400">
                  Sentiment
                </p>

                <p className="mt-2 text-2xl font-bold text-red-400">

                  {data.initial_analysis.sentiment}

                </p>

              </div>

              {/* Confidence */}

              <div className="rounded-xl bg-zinc-900 p-4">

                <p className="text-zinc-400">
                  Confidence
                </p>

                <p className="mt-2 text-2xl font-bold">

                  {(data.initial_analysis.confidence * 100).toFixed(0)}%

                </p>

              </div>

            </div>

          </div>

        </div>

      )}

    </main>
  );
}