namespace Pat

open System
open System.IO
open System.Runtime.InteropServices

module Binder =
    let bind (binaryPath: string) : Cabi option =
        let filename = Path.GetFileName binaryPath
        let skip = [ "c"; "cpp"; "rust"; "go"; "swift"; "haskell"; "zig"; "v" ]
        if List.contains filename skip then None
        else
            let mutable handle = IntPtr.Zero
            if not (NativeLibrary.TryLoad(binaryPath, &handle)) then None
            else
                let mutable morePtr = IntPtr.Zero
                let mutable pumpPtr = IntPtr.Zero
                let mutable lessPtr = IntPtr.Zero
                if NativeLibrary.TryGetExport(handle, "More", &morePtr) &&
                   NativeLibrary.TryGetExport(handle, "Pump", &pumpPtr) &&
                   NativeLibrary.TryGetExport(handle, "Less", &lessPtr) then
                    Some {
                        Name = filename
                        Path = binaryPath
                        Lib = handle
                        More = Marshal.GetDelegateForFunctionPointer<MoreFn>(morePtr)
                        Pump = Marshal.GetDelegateForFunctionPointer<PumpFn>(pumpPtr)
                        Less = Marshal.GetDelegateForFunctionPointer<LessFn>(lessPtr)
                    }
                else
                    NativeLibrary.Free handle
                    None
