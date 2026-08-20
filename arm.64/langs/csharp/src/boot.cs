namespace Pat;

public static class Booter
{
    public static List<Cabi> Boot(string? targetDir = null)
    {
        var engines = new List<Cabi>();
        string? envLib = Environment.GetEnvironmentVariable("PAT_LIB");
        if (!string.IsNullOrEmpty(envLib))
        {
            var c = Binder.Bind(envLib);
            if (c != null)
            {
                engines.Add(c);
                return engines;
            }
        }

        foreach (var file in Finder.Find(targetDir))
        {
            var c = Binder.Bind(file);
            if (c != null) engines.Add(c);
        }
        return engines;
    }
}
