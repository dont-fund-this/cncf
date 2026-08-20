package libs

import "unsafe"

func LibBind(handle unsafe.Pointer, symbol string) unsafe.Pointer {
	if handle == nil || len(symbol) == 0 {
		return nil
	}
	return nil
}
