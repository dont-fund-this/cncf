package pat;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.util.List;

public class main {
    public static void main(String[] args) {
        String targetDir = args.length > 0 ? args[0] : null;

        try (Arena arena = Arena.ofShared()) {
            List<type.Cabi> dist = boot.boot(targetDir, arena);

            if (!dist.isEmpty()) {
                List<type.Triplet> trips = trip.trip();
                for (type.Cabi d : dist) {
                    for (type.Triplet t : trips) {
                        try {
                            MemorySegment addr = arena.allocateFrom(t.address);
                            MemorySegment pay = arena.allocateFrom(t.payload);
                            MemorySegment opt = arena.allocateFrom(t.options);
                            d.pump.invoke(addr, pay, opt);
                        } catch (Throwable ignored) {}
                    }
                }
            }

            System.out.printf("{\n  \"lang\": \"java\",\n  \"status\": \"ready\",\n  \"engines\": %d\n}\n", dist.size());
        }
    }
}
