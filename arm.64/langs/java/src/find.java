package pat;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class find {
    public static List<String> find(String targetDir) {
        String dir = targetDir;
        if (dir == null || dir.isEmpty()) {
            dir = System.getenv("DIST_DIR");
            if (dir == null || dir.isEmpty()) {
                String[] candidates = {
                    "../../../dist",
                    "../../dist",
                    "dist"
                };
                dir = "dist";
                for (String c : candidates) {
                    if (Files.isDirectory(Paths.get(c))) {
                        dir = Paths.get(c).toAbsolutePath().normalize().toString();
                        break;
                    }
                }
            }
        }

        List<String> files = new ArrayList<>();
        File folder = new File(dir);
        if (folder.exists() && folder.isDirectory()) {
            File[] listOfFiles = folder.listFiles();
            if (listOfFiles != null) {
                for (File file : listOfFiles) {
                    if (file.isFile() && !file.getName().equals(".DS_Store")) {
                        files.add(file.getAbsolutePath());
                    }
                }
            }
        }
        return files;
    }
}
