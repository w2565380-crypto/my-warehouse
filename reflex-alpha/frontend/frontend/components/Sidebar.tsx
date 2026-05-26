interface Props {
  thinkingSteps: string[];
}

export default function Sidebar({
  thinkingSteps
}: Props) {

  return (

    <div
      className="
        h-full
        bg-slate-900
        border-r
        border-slate-800
        p-5
        overflow-y-auto
      "
    >

      <h2
        className="
          text-white
          text-lg
          font-semibold
          mb-5
        "
      >
        Agent Thinking
      </h2>

      <div
        className="
          space-y-3
        "
      >

        {thinkingSteps.map(
          (step, index) => (

            <div
              key={index}
              className="
                bg-slate-800
                border
                border-slate-700
                rounded-xl
                p-3
                text-sm
                text-green-400
                font-mono
              "
            >
              {step}
            </div>
          )
        )}

      </div>

    </div>
  );
}