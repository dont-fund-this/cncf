using System;
using System.Runtime.InteropServices;

namespace Pat {
    public delegate bool FitFn(IntPtr address, IntPtr payload, IntPtr options);
    public delegate int FunFn(IntPtr address, IntPtr payload, IntPtr options);

    [StructLayout(LayoutKind.Sequential)]
    public struct Def {
        public IntPtr sid;
        public IntPtr tag;
        public IntPtr fit;
        public IntPtr fun;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int MoreFn(Def def);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int PumpFn(
        [MarshalAs(UnmanagedType.LPUTF8Str)] string address,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string payload,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string options);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int LessFn(Def def);

    public class Cabi {
        public string Name { get; set; } = "";
        public string Path { get; set; } = "";
        public IntPtr Lib { get; set; }
        public MoreFn? More { get; set; }
        public PumpFn? Pump { get; set; }
        public LessFn? Less { get; set; }
    }

    public class Triplet {
        public string Address { get; set; }
        public string Payload { get; set; }
        public string Options { get; set; }

        public Triplet(string address, string payload = "{}", string options = "{\"once\":true}") {
            Address = address;
            Payload = payload;
            Options = options;
        }
    }
}
