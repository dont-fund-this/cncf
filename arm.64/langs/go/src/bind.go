package main

/*
#include <dlfcn.h>
#include <stdlib.h>
*/
import "C"
import (
	"path/filepath"
	"unsafe"
)

func bind(binaryPath string) *Cabi {
	filename := filepath.Base(binaryPath)
	skips := map[string]bool{"c": true, "cpp": true, "rust": true, "go": true, "swift": true, "haskell": true, "zig": true, "v": true, "slint_sample": true}
	if skips[filename] {
		return nil
	}

	cPath := C.CString(binaryPath)
	defer C.free(unsafe.Pointer(cPath))

	handle := C.dlopen(cPath, C.RTLD_LAZY|C.RTLD_LOCAL)
	if handle == nil {
		return nil
	}

	pumpSym := C.CString("Pump")
	defer C.free(unsafe.Pointer(pumpSym))

	pumpFn := C.dlsym(handle, pumpSym)
	if pumpFn == nil {
		C.dlclose(handle)
		return nil
	}

	return &Cabi{
		Name:   filename,
		Path:   binaryPath,
		Handle: handle,
		PumpFn: pumpFn,
	}
}
