from neo4j import GraphDatabase
import os

from dotenv import load_dotenv

load_dotenv()

driver = GraphDatabase.driver(
    os.getenv("NEO4J_URI"),
    auth=(
        os.getenv("NEO4J_USERNAME"),
        os.getenv("NEO4J_PASSWORD")
    )
)

def retrieve_related_knowledge(entity_name):

    query = """
    MATCH (a)-[r]->(b)
    WHERE a.name = $entity_name
       OR b.name = $entity_name

    RETURN
        a.name AS source,
        type(r) AS relation,
        b.name AS target
    LIMIT 10
    """

    with driver.session() as session:

        results = session.run(
            query,
            entity_name=entity_name
        )

        knowledge = []

        for record in results:

            knowledge.append({

                "source": record["source"],
                "relation": record["relation"],
                "target": record["target"]
            })

        return knowledge