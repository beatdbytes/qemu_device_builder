from dataclasses import dataclass
from typing import List


@dataclass(init=True, repr=True, eq=True)
class BitField:
    Name: str
    Start: str
    End: str
    Width: str
    Access: str


@dataclass(init=True, repr=True, eq=True)
class SFR:
    Name: str
    Desc: str
    Addr: str
    RVal: str
    BitFields: List[BitField]
    Writable: bool


@dataclass(init=True, repr=True, eq=True)
class BLOCK:
    Name: str
    InstanceName: str
    Start: str
    End: str
    Size: str
    SfrList: List[SFR]
