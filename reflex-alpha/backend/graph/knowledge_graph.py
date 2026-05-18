import networkx as nx

graph = nx.Graph()

def add_relationship(entity1, relation, entity2):

    graph.add_node(entity1)

    graph.add_node(entity2)

    graph.add_edge(
        entity1,
        entity2,
        relation=relation
    )

def get_related_entities(entity):

    if entity not in graph:
        return []

    return list(graph.neighbors(entity))