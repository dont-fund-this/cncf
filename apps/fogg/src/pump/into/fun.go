package into

import (
	"fogg/src/type"
	"strings"
	"unsafe"
)

func CStr(p unsafe.Pointer) string {
	if p == nil {
		return ""
	}
	buf := (*[1 << 20]byte)(p)
	var i int
	for buf[i] != 0 {
		i++
	}
	return string(buf[:i])
}

func StringToC(s string) unsafe.Pointer {
	b := append([]byte(s), 0)
	return unsafe.Pointer(&b[0])
}

func GetInto(options types.Options) types.Address {
	if options == nil {
		return nil
	}
	opt := CStr(options)
	if idx := strings.Index(opt, "into:"); idx != -1 {
		return unsafe.Pointer(uintptr(options) + uintptr(idx+5))
	}
	if strings.HasPrefix(opt, "{") || strings.HasPrefix(opt, "[") {
		return nil
	}
	if opt != "" && opt != "once" && opt != "many" && opt != "none" {
		return options
	}
	return nil
}

func HasVerb(options types.Options, verb string) bool {
	if options == nil {
		return verb == "GET"
	}
	opt := CStr(options)
	if idx := strings.Index(opt, "verb:"); idx != -1 {
		rest := opt[idx+5:]
		if comma := strings.Index(rest, ","); comma != -1 {
			rest = rest[:comma]
		}
		return rest == verb
	}
	return verb == "GET"
}
