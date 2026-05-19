from graph.knowledge_graph import graph

def find_competitors(company):

    competitors = []

    for neighbor in graph.neighbors(company):

        relation = graph.edges[
            company,
            neighbor
        ]["relation"]

        if relation == "competes_with":
            competitors.append(neighbor)

    return competitors