import os

from dotenv import load_dotenv

from neo4j import GraphDatabase

load_dotenv()

URI = os.getenv("NEO4J_URI")

USERNAME = os.getenv(
    "NEO4J_USERNAME"
)

PASSWORD = os.getenv(
    "NEO4J_PASSWORD"
)

driver = GraphDatabase.driver(
    URI,
    auth=(USERNAME, PASSWORD)
)




def add_relationship(
    source,
    relation,
    target
):

    query = f"""
    MERGE (a:Entity {{name: $source}})
    MERGE (b:Entity {{name: $target}})
    MERGE (a)-[:{relation}]->(b)
    """

    with driver.session() as session:

        session.run(
            query,
            source=source,
            target=target
        )