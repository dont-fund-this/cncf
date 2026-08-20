package pat

import java.io.File
import java.lang.foreign.*
import java.lang.invoke.MethodHandle

fun bind(binaryPath: String, arena: Arena): Cabi? {
    val filename = File(binaryPath).name
    val skips = listOf("c", "cpp", "rust", "go", "swift", "haskell", "zig", "v", "slint_sample")
    if (skips.contains(filename)) return null

    return try {
        val lib = SymbolLookup.libraryLookup(binaryPath, arena)
        val pumpSeg = lib.find("Pump").orElse(null) ?: return null

        val desc = FunctionDescriptor.of(
            ValueLayout.JAVA_INT,
            ValueLayout.ADDRESS,
            ValueLayout.ADDRESS,
            ValueLayout.ADDRESS
        )
        val pump = Linker.nativeLinker().downcallHandle(pumpSeg, desc)
        Cabi(name = filename, path = binaryPath, pump = pump)
    } catch (e: Exception) {
        null
    }
}
