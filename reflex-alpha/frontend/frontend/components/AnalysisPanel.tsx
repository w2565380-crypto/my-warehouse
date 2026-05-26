interface Props {
  result: any;
}

export default function AnalysisPanel({
  result
}: Props) {

  if (!result) return null;

  return (

    <div
      className="
        bg-slate-900
        border
        border-slate-800
        rounded-2xl
        p-6
        mt-6
      "
    >

      <h2
        className="
          text-white
          text-2xl
          font-bold
          mb-4
        "
      >
        Market Analysis
      </h2>

      <div
        className="
          space-y-5
        "
      >

        <div>

          <p
            className="
              text-slate-400
              text-sm
              mb-1
            "
          >
            Headline
          </p>

          <p
            className="
              text-white
            "
          >
            {result.headline}
          </p>

        </div>

        <div>

          <p
            className="
              text-slate-400
              text-sm
              mb-1
            "
          >
            Sentiment
          </p>

          <p
            className="
              text-green-400
              font-semibold
            "
          >
            {result.initial_analysis.sentiment}
          </p>

        </div>

        <div>

          <p
            className="
              text-slate-400
              text-sm
              mb-1
            "
          >
            Confidence
          </p>

          <p
            className="
              text-white
            "
          >
            {result.initial_analysis.confidence}
          </p>

        </div>

        <div>

          <p
            className="
              text-slate-400
              text-sm
              mb-1
            "
          >
            Reasoning
          </p>

          <p
            className="
              text-slate-300
              leading-7
            "
          >
            {result.initial_analysis.reasoning}
          </p>

        </div>

      </div>

    </div>
  );
}