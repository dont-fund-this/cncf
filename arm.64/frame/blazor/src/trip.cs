using System.Collections.Generic;

namespace Pat {
    public static class Tripper {
        public static List<Triplet> Trip() {
            return new List<Triplet> {
                new Triplet("/version", "{}", "{\"once\":true}"),
                new Triplet("/storage", "{}", "{\"once\":true}"),
                new Triplet("sql.help", "{}", "{\"once\":true}")
            };
        }
    }
}
