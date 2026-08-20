namespace Pat;

public static class Tripper
{
    public static List<Triplet> Trip()
    {
        return new List<Triplet>
        {
            new Triplet { Address = "/version", Payload = "{}", Options = "{\"once\":true}" },
            new Triplet { Address = "/storage", Payload = "{}", Options = "{\"once\":true}" },
            new Triplet { Address = "sql.help", Payload = "{}", Options = "{\"once\":true}" }
        };
    }
}
