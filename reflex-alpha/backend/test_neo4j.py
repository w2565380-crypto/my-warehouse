from graph.neo4j_graph import (
    add_relationship
)

add_relationship(
    "NVIDIA",
    "COMPETES_WITH",
    "AMD"
)

print("done")