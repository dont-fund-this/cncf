from typing import List
from type import Triplet

def trip() -> List[Triplet]:
    return [
        Triplet(address="/version", payload="{}", options='{"once":true}'),
        Triplet(address="/storage", payload="{}", options='{"once":true}'),
        Triplet(address="sql.help", payload="{}", options='{"once":true}')
    ]
