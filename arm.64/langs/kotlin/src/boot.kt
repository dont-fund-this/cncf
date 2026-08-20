package pat

import java.lang.foreign.Arena

fun boot(targetDir: String? = null, arena: Arena): List<Cabi> {
    val engines = mutableListOf<Cabi>()
    val envLib = System.getenv("PAT_LIB")
    if (!envLib.isNullOrEmpty()) {
        val c = bind(envLib, arena)
        if (c != null) {
            engines.add(c)
            return engines
        }
    }

    val files = find(targetDir)
    for (file in files) {
        val c = bind(file, arena)
        if (c != null) {
            engines.add(c)
        }
    }
    return engines
}
