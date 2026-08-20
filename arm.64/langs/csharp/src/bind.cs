using System.Runtime.InteropServices;

namespace Pat;

public static class Binder
{
    public static Cabi? Bind(string binaryPath)
    {
        string filename = Path.GetFileName(binaryPath);
        string[] skips = { "c", "cpp", "rust", "go", "swift", "haskell", "zig", "v", "slint_sample" };
        if (skips.Contains(filename)) return null;

        if (!NativeLibrary.TryLoad(binaryPath, out IntPtr handle)) return null;
        if (!NativeLibrary.TryGetExport(handle, "Pump", out IntPtr pumpPtr))
        {
            NativeLibrary.Free(handle);
            return null;
        }

        var pump = Marshal.GetDelegateForFunctionPointer<PumpDelegate>(pumpPtr);
        NativeLibrary.TryGetExport(handle, "More", out IntPtr morePtr);
        NativeLibrary.TryGetExport(handle, "Less", out IntPtr lessPtr);

        return new Cabi
        {
            Name = filename,
            Path = binaryPath,
            Handle = handle,
            More = morePtr,
            Pump = pump,
            Less = lessPtr
        };
    }
}
