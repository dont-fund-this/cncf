namespace Pat;

public static class Finder
{
    public static List<string> Find(string? targetDir = null)
    {
        string? dir = targetDir;
        if (string.IsNullOrEmpty(dir))
        {
            dir = Environment.GetEnvironmentVariable("DIST_DIR");
            if (string.IsNullOrEmpty(dir))
            {
                string[] candidates = {
                    Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "../../../dist")),
                    Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "../../dist")),
                    Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), "dist")),
                    "dist"
                };
                dir = candidates.FirstOrDefault(Directory.Exists) ?? "dist";
            }
        }

        if (!Directory.Exists(dir)) return new List<string>();

        return Directory.GetFiles(dir)
            .Where(f => Path.GetFileName(f) != ".DS_Store")
            .ToList();
    }
}
