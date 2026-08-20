namespace Pat

open System
open System.Runtime.InteropServices

type PumpDelegate = delegate of string * string * string -> int

type Cabi = {
    Name: string
    Path: string
    Handle: IntPtr
    Pump: PumpDelegate
}

type Triplet = {
    Address: string
    Payload: string
    Options: string
}
