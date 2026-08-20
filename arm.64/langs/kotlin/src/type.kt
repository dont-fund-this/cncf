package pat

import java.lang.invoke.MethodHandle

data class Def(val sid: String, val tag: String)
data class Cabi(val name: String, val path: String, val pump: MethodHandle)
data class Triplet(val address: String, val payload: String, val options: String)
