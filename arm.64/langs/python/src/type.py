from ctypes import Structure, c_char_p, c_int, c_bool, CFUNCTYPE
from dataclasses import dataclass
from typing import Optional, Any

Address = c_char_p
Payload = c_char_p
Options = c_char_p
Sid = c_char_p
Tag = c_char_p

Fit = CFUNCTYPE(c_bool, Address, Payload, Options)
Fun = CFUNCTYPE(c_int, Address, Payload, Options)

class Def(Structure):
    _fields_ = [
        ("sid", Sid),
        ("tag", Tag),
        ("fit", Fit),
        ("fun", Fun)
    ]

@dataclass
class Cabi:
    name: str
    path: str
    lib: Any
    More: Any
    Pump: Any
    Less: Any

@dataclass
class Triplet:
    address: str
    payload: str
    options: str
