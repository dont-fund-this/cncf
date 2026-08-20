namespace Pat;

public static class Program
{
    public static void Main(string[] args)
    {
        string? targetDir = args.Length > 0 ? args[0] : null;
        var dist = Booter.Boot(targetDir);

        if (dist.Count > 0)
        {
            var trips = Tripper.Trip();
            foreach (var d in dist)
            {
                if (d.Pump != null)
                {
                    foreach (var t in trips)
                    {
                        d.Pump(t.Address, t.Payload, t.Options);
                    }
                }
            }
        }

        Console.WriteLine($"{{\n  \"lang\": \"csharp\",\n  \"status\": \"ready\",\n  \"engines\": {dist.Count}\n}}");
    }
}
