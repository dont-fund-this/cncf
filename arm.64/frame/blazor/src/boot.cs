using System;
using System.Collections.Generic;

namespace Pat {
    public static class Booter {
        public static List<Cabi> Boot(string? targetDir = null) {
            var engines = new List<Cabi>();

            string? envLib = Environment.GetEnvironmentVariable("PAT_LIB");
            if (!string.IsNullOrEmpty(envLib)) {
                var bound = Binder.Bind(envLib);
                if (bound != null) {
                    engines.Add(bound);
                    return engines;
                }
            }

            foreach (var bin in Finder.Find(targetDir)) {
                var bound = Binder.Bind(bin);
                if (bound != null) {
                    engines.Add(bound);
                }
            }

            return engines;
        }
    }
}
