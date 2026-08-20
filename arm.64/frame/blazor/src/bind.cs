using System;
using System.IO;
using System.Runtime.InteropServices;

namespace Pat {
    public static class Binder {
        public static Cabi? Bind(string binaryPath) {
            string filename = Path.GetFileName(binaryPath);
            if (filename == "c" || filename == "cpp" || filename == "rust" ||
                filename == "go" || filename == "swift" || filename == "haskell" ||
                filename == "zig" || filename == "v") {
                return null;
            }

            if (!NativeLibrary.TryLoad(binaryPath, out IntPtr handle)) {
                return null;
            }

            if (!NativeLibrary.TryGetExport(handle, "More", out IntPtr morePtr) ||
                !NativeLibrary.TryGetExport(handle, "Pump", out IntPtr pumpPtr) ||
                !NativeLibrary.TryGetExport(handle, "Less", out IntPtr lessPtr)) {
                NativeLibrary.Free(handle);
                return null;
            }

            return new Cabi {
                Name = filename,
                Path = binaryPath,
                Lib = handle,
                More = Marshal.GetDelegateForFunctionPointer<MoreFn>(morePtr),
                Pump = Marshal.GetDelegateForFunctionPointer<PumpFn>(pumpPtr),
                Less = Marshal.GetDelegateForFunctionPointer<LessFn>(lessPtr)
            };
        }
    }
}
