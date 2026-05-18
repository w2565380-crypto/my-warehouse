"use client";

import { useEffect, useState } from "react";

export default function Home() {
  const [data, setData] = useState<any>(null);

  useEffect(() => {
    fetch("http://127.0.0.1:8000/analyze")
      .then((res) => res.json())
      .then((data) => {
        setData(data);
      });
  }, []);

  return (
    <main className="min-h-screen bg-black text-white p-6">
      <h1 className="text-4xl font-bold">
        ReflexAlpha
      </h1>

      <p className="mt-2 text-zinc-400">
        Reflective Multi-Agent Financial Intelligence Platform
      </p>

      {!data ? (
        <p className="mt-10">Loading AI analysis...</p>
      ) : (
        <div className="mt-10 grid grid-cols-3 gap-4">

          {/* News */}
          <div className="rounded-2xl border border-zinc-800 p-4">
            <h2 className="text-xl font-semibold mb-4">
              News
            </h2>

            <div className="rounded-xl bg-zinc-900 p-4">
              {data.headline}
            </div>
          </div>

          {/* Analysis */}
          <div className="rounded-2xl border border-zinc-800 p-4">
            <h2 className="text-xl font-semibold mb-4">
              AI Analysis
            </h2>

            <div className="rounded-xl bg-zinc-900 p-4">
              <div className="space-y-4">

                <div>
                  <p className="text-zinc-400">
                    Reasoning
                  </p>

                  <p className="mt-2 text-zinc-200">
                    <div className="space-y-4">

                      <div>
                        <p className="text-zinc-400">
                          Initial Analysis
                        </p>

                        <p className="mt-2 text-zinc-200">
                          {data.initial_analysis.reasoning}
                        </p>
                      </div>

                      <div>
                        <p className="text-zinc-400">
                          Reflection Agent
                        </p>

                        <p className="mt-2 text-yellow-300">
                          {data.reflection}
                        </p>
                      </div>

                    </div>
                  </p>
                </div>

              </div>
            </div>
          </div>

          {/* Market Impact */}
          <div className="rounded-2xl border border-zinc-800 p-4">
            <h2 className="text-xl font-semibold mb-4">
              Market Impact
            </h2>

            <div className="space-y-4">

              <div className="rounded-xl bg-zinc-900 p-4">
                <p className="text-zinc-400">
                  Sentiment
                </p>

                <p className="mt-2 text-2xl font-bold text-red-400">
                  {data.initial_analysis.sentiment}
                </p>
              </div>

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