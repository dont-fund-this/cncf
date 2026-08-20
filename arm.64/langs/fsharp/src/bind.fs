namespace Pat

open System
open System.IO
open System.Runtime.InteropServices

module Binder =
    let bind (binaryPath: string) : Cabi option =
        let filename = Path.GetFileName binaryPath
        let skips = [ "c"; "cpp"; "rust"; "go"; "swift"; "haskell"; "zig"; "v"; "slint_sample" ]
        if List.contains filename skips then None
        else
            let mutable handle = IntPtr.Zero
            if NativeLibrary.TryLoad(binaryPath, &handle) then
                let mutable pumpPtr = IntPtr.Zero
                if NativeLibrary.TryGetExport(handle, "Pump", &pumpPtr) then
                    let pump = Marshal.GetDelegateForFunctionPointer<PumpDelegate>(pumpPtr)
                    Some { Name = filename; Path = binaryPath; Handle = handle; Pump = pump }
                else
                    NativeLibrary.Free handle
                    None
            else None
