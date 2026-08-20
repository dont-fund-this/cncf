package pat;

import java.util.Arrays;
import java.util.List;

public class trip {
    public static List<type.Triplet> trip() {
        return Arrays.asList(
            new type.Triplet("/version", "{}", "{\"once\":true}"),
            new type.Triplet("/storage", "{}", "{\"once\":true}"),
            new type.Triplet("sql.help", "{}", "{\"once\":true}")
        );
    }
}
