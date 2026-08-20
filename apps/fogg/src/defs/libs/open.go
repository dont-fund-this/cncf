package libs

import "unsafe"

func LibOpen(path string) unsafe.Pointer {
	if len(path) == 0 {
		return nil
	}
	return nil
}
