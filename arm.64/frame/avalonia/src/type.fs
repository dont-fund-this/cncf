namespace Pat

open System
open System.Runtime.InteropServices

type FitFn = delegate of address: nativeint * payload: nativeint * options: nativeint -> bool
type FunFn = delegate of address: nativeint * payload: nativeint * options: nativeint -> int

[<Struct; StructLayout(LayoutKind.Sequential)>]
type Def = {
    sid: nativeint
    tag: nativeint
    fit: nativeint
    fun_: nativeint
}

[<UnmanagedFunctionPointer(CallingConvention.Cdecl)>]
type MoreFn = delegate of def_: Def -> int

[<UnmanagedFunctionPointer(CallingConvention.Cdecl)>]
type PumpFn = delegate of [<MarshalAs(UnmanagedType.LPUTF8Str)>] address: string * [<MarshalAs(UnmanagedType.LPUTF8Str)>] payload: string * [<MarshalAs(UnmanagedType.LPUTF8Str)>] options: string -> int

[<UnmanagedFunctionPointer(CallingConvention.Cdecl)>]
type LessFn = delegate of def_: Def -> int

type Cabi = {
    Name: string
    Path: string
    Lib: nativeint
    More: MoreFn
    Pump: PumpFn
    Less: LessFn
}

type Triplet = {
    Address: string
    Payload: string
    Options: string
}
