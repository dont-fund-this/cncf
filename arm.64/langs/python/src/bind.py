import ctypes
from pathlib import Path
from typing import Optional
from type import Cabi, Def

def bind(binary_path: str) -> Optional[Cabi]:
    filename = Path(binary_path).name
    if filename in ["c", "cpp", "rust", "go", "swift", "haskell", "zig", "v", "slint_sample"]:
        return None

    try:
        lib = ctypes.CDLL(binary_path)
        
        more_fn = None
        if hasattr(lib, "More"):
            more_fn = lib.More
            more_fn.argtypes = [ctypes.POINTER(Def)]
            more_fn.restype = ctypes.c_int

        pump_fn = getattr(lib, "Pump", None)
        if not pump_fn:
            return None
        pump_fn.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
        pump_fn.restype = ctypes.c_int

        less_fn = None
        if hasattr(lib, "Less"):
            less_fn = lib.Less
            less_fn.argtypes = [ctypes.POINTER(Def)]
            less_fn.restype = ctypes.c_int

        return Cabi(
            name=filename,
            path=binary_path,
            lib=lib,
            More=more_fn,
            Pump=pump_fn,
            Less=less_fn
        )
    except Exception:
        return None
