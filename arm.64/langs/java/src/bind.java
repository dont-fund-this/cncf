package pat;

import java.io.File;
import java.lang.foreign.*;
import java.lang.invoke.MethodHandle;
import java.util.Arrays;
import java.util.List;

public class bind {
    public static type.Cabi bind(String binaryPath, Arena arena) {
        String filename = new File(binaryPath).getName();
        List<String> skips = Arrays.asList("c", "cpp", "rust", "go", "swift", "haskell", "zig", "v", "slint_sample");
        if (skips.contains(filename)) return null;

        try {
            SymbolLookup lib = SymbolLookup.libraryLookup(binaryPath, arena);
            MemorySegment pumpSeg = lib.find("Pump").orElse(null);
            if (pumpSeg == null) return null;

            FunctionDescriptor desc = FunctionDescriptor.of(
                ValueLayout.JAVA_INT,
                ValueLayout.ADDRESS,
                ValueLayout.ADDRESS,
                ValueLayout.ADDRESS
            );
            MethodHandle pump = Linker.nativeLinker().downcallHandle(pumpSeg, desc);

            type.Cabi cabi = new type.Cabi();
            cabi.name = filename;
            cabi.path = binaryPath;
            cabi.pump = pump;
            return cabi;
        } catch (Exception e) {
            return null;
        }
    }
}
