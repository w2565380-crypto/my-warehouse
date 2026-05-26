export default function Header() {

  return (

    <div
      className="
        w-full
        border-b
        border-slate-800
        px-8
        py-5
        flex
        items-center
        justify-between
        bg-slate-950
      "
    >

      <div>

        <h1
          className="
            text-3xl
            font-bold
            text-white
          "
        >
          ReflexAlpha
        </h1>

        <p
          className="
            text-slate-400
            text-sm
            mt-1
          "
        >
          Graph-Augmented Financial AI Agent
        </p>

      </div>

      <button
        className="
          bg-blue-600
          hover:bg-blue-500
          transition
          px-5
          py-2
          rounded-xl
          text-white
          font-medium
        "
      >
        Run Analysis
      </button>

    </div>
  );
}