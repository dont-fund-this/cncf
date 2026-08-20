package pat;

import java.lang.invoke.MethodHandle;

public class type {
    public static class Def {
        public String sid;
        public String tag;
    }

    public static class Cabi {
        public String name;
        public String path;
        public MethodHandle pump;
    }

    public static class Triplet {
        public String address;
        public String payload;
        public String options;

        public Triplet(String address, String payload, String options) {
            this.address = address;
            this.payload = payload;
            this.options = options;
        }
    }
}
