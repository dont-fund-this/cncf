using System;

namespace Pat {
    public class Program {
        public static void Main(string[] args) {
            string? targetDir = args.Length > 0 ? args[0] : null;
            var dist = Booter.Boot(targetDir);

            if (dist.Count > 0) {
                var trips = Tripper.Trip();
                foreach (var d in dist) {
                    foreach (var t in trips) {
                        d.Pump?.Invoke(t.Address, t.Payload, t.Options);
                    }
                }
            }

            Console.WriteLine($"{{\n  \"framework\": \"blazor\",\n  \"status\": \"ready\",\n  \"engines\": {dist.Count}\n}}");
        }
    }
}
