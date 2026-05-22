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




def add_typed_relationship(
    source_name,
    source_type,
    relation,
    target_name,
    target_type
):

    query = f"""
    MERGE (a:{source_type} {{name: $source_name}})
    MERGE (b:{target_type} {{name: $target_name}})
    MERGE (a)-[:{relation}]->(b)
    """

    with driver.session() as session:

        session.run(
            query,
            source_name=source_name,
            target_name=target_name
        )