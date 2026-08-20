import os
from typing import List, Optional
from type import Cabi
from find import find
from bind import bind

def boot(target_dir: Optional[str] = None) -> List[Cabi]:
    engines = []
    env_lib = os.environ.get("PAT_LIB")
    if env_lib:
        c = bind(env_lib)
        if c:
            engines.append(c)
            return engines

    files = find(target_dir)
    for f in files:
        c = bind(f)
        if c:
            engines.append(c)
    return engines
