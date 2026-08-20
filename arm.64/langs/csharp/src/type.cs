using System.Runtime.InteropServices;

namespace Pat;

public struct Def
{
    public IntPtr Sid;
    public IntPtr Tag;
    public IntPtr Fit;
    public IntPtr Fun;
}

public delegate int PumpDelegate(string address, string payload, string options);

public class Cabi
{
    public string Name { get; set; } = "";
    public string Path { get; set; } = "";
    public IntPtr Handle { get; set; }
    public IntPtr More { get; set; }
    public PumpDelegate? Pump { get; set; }
    public IntPtr Less { get; set; }
}

public class Triplet
{
    public string Address { get; set; } = "";
    public string Payload { get; set; } = "";
    public string Options { get; set; } = "";
}
