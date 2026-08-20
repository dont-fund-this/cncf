import os
import sys
from pathlib import Path
from typing import List, Optional

def find(target_dir: Optional[str] = None) -> List[str]:
    if target_dir:
        d = Path(target_dir)
    elif os.environ.get("DIST_DIR"):
        d = Path(os.environ["DIST_DIR"])
    else:
        exe = Path(sys.argv[0]).resolve()
        candidates = [
            exe.parent / "../../../dist",
            exe.parent / "../../dist",
            Path("dist").resolve(),
            Path("../../dist").resolve(),
            Path("../../../dist").resolve(),
        ]
        d = Path("dist").resolve()
        for c in candidates:
            if c.exists() and c.is_dir():
                d = c.resolve()
                break

    if not d.exists():
        return []

    try:
        return [
            str(p) for p in d.iterdir()
            if p.is_file() and p.name != ".DS_Store"
        ]
    except Exception:
        return []
