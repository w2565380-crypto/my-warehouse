"use client";

import ReactFlow, {
  Background,
  Controls,
  MiniMap
} from "reactflow";

import "reactflow/dist/style.css";
import styles from "./GraphView.module.css";

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

      className: styles.node
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

    <div className={styles.container}>

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