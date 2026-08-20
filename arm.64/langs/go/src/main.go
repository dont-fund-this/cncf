package main

/*
#include <stdlib.h>
typedef int (*PumpFn)(const char*, const char*, const char*);
static inline int call_pump(void* f, const char* a, const char* p, const char* o) {
    return ((PumpFn)f)(a, p, o);
}
*/
import "C"
import (
	"fmt"
	"os"
	"unsafe"
)

func main() {
	targetDir := ""
	if len(os.Args) > 1 {
		targetDir = os.Args[1]
	}

	dist := boot(targetDir)
	if len(dist) > 0 {
		trips := trip()
		for _, d := range dist {
			for _, t := range trips {
				cA := C.CString(t.Address)
				cP := C.CString(t.Payload)
				cO := C.CString(t.Options)
				C.call_pump(d.PumpFn, cA, cP, cO)
				C.free(unsafe.Pointer(cA))
				C.free(unsafe.Pointer(cP))
				C.free(unsafe.Pointer(cO))
			}
		}
	}

	fmt.Printf("{\n  \"lang\": \"go\",\n  \"status\": \"ready\",\n  \"engines\": %d\n}\n", len(dist))
}
