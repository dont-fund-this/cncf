package pat;

import java.lang.foreign.Arena;
import java.util.ArrayList;
import java.util.List;

public class boot {
    public static List<type.Cabi> boot(String targetDir, Arena arena) {
        List<type.Cabi> engines = new ArrayList<>();
        String envLib = System.getenv("PAT_LIB");
        if (envLib != null && !envLib.isEmpty()) {
            type.Cabi c = bind.bind(envLib, arena);
            if (c != null) {
                engines.add(c);
                return engines;
            }
        }

        List<String> files = find.find(targetDir);
        for (String file : files) {
            type.Cabi c = bind.bind(file, arena);
            if (c != null) {
                engines.add(c);
            }
        }
        return engines;
    }
}
