using System;
using System.Collections.Generic;
using System.IO;

namespace Pat {
    public static class Finder {
        public static List<string> Find(string? targetDir = null) {
            string? dir = targetDir;
            if (string.IsNullOrEmpty(dir)) {
                string? envDir = Environment.GetEnvironmentVariable("DIST_DIR");
                if (!string.IsNullOrEmpty(envDir)) {
                    dir = envDir;
                } else {
                    string[] candidates = new string[] {
                        Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "../../../dist")),
                        Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "../../dist")),
                        "dist",
                        "../../dist"
                    };
                    dir = "dist";
                    foreach (var c in candidates) {
                        if (Directory.Exists(c)) {
                            dir = c;
                            break;
                        }
                    }
                }
            }

            var files = new List<string>();
            if (!Directory.Exists(dir)) return files;

            try {
                foreach (var file in Directory.GetFiles(dir)) {
                    if (Path.GetFileName(file) != ".DS_Store") {
                        files.Add(Path.GetFullPath(file));
                    }
                }
            } catch {}

            return files;
        }
    }
}
