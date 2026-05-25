"use client";

import ReactFlow, {
  Background,
  Controls,
  MiniMap
} from "reactflow";

import "reactflow/dist/style.css";

interface Relationship {
  source: string;
  relation: string;
  target: string;
}

interface Props {
  relationships: Relationship[];
}

export default function GraphView({
  relationships
}: Props) {

  // --------------------------------
  // Build Nodes
  // --------------------------------

  const nodeSet = new Set<string>();

  relationships.forEach((rel) => {

    nodeSet.add(rel.source);

    nodeSet.add(rel.target);
  });

  const nodeArray = Array.from(nodeSet);

  const nodes = nodeArray.map(
    (node, index) => ({

      id: node,

      data: {
        label: node
      },

      position: {
        x: (index % 4) * 250,
        y: Math.floor(index / 4) * 150
      },

      style: {

        background: "#111827",

        color: "white",

        border: "1px solid #374151",

        borderRadius: "12px",

        padding: 10,

        width: 160,

        fontSize: 14
      }
    })
  );

  // --------------------------------
  // Build Edges
  // --------------------------------

  const edges = relationships.map(
    (rel, index) => ({

      id: `edge-${index}`,

      source: rel.source,

      target: rel.target,

      label: rel.relation,

      animated: true,

      style: {
        stroke: "#64748b"
      },

      labelStyle: {
        fill: "#94a3b8",
        fontSize: 12
      }
    })
  );

  return (

    <div
      style={{
        width: "100%",
        height: "700px",
        background: "#0f172a",
        borderRadius: "16px",
        overflow: "hidden",
        border: "1px solid #1e293b"
      }}
    >

      <ReactFlow

        nodes={nodes}

        edges={edges}

        fitView

      >

        <MiniMap />

        <Controls />

        <Background />

      </ReactFlow>

    </div>
  );
}