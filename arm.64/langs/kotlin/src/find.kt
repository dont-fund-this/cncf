package pat

import java.io.File
import java.nio.file.Files
import java.nio.file.Paths

fun find(targetDir: String? = null): List<String> {
    var dir = targetDir
    if (dir.isNullOrEmpty()) {
        dir = System.getenv("DIST_DIR")
        if (dir.isNullOrEmpty()) {
            val candidates = listOf("../../../dist", "../../dist", "dist")
            dir = "dist"
            for (c in candidates) {
                if (Files.isDirectory(Paths.get(c))) {
                    dir = Paths.get(c).toAbsolutePath().normalize().toString()
                    break
                }
            }
        }
    }

    val files = mutableListOf<String>()
    val folder = File(dir)
    if (folder.exists() && folder.isDirectory) {
        folder.listFiles()?.forEach { file ->
            if (file.isFile && file.name != ".DS_Store") {
                files.add(file.absolutePath)
            }
        }
    }
    return files
}
