package want

import (
	"fogg/src/pump/into"
	"fogg/src/type"
	"strings"
)

func Once(options types.Options) bool {
	if options == nil {
		return false
	}
	opt := into.CStr(options)
	return opt == "once" || strings.HasPrefix(opt, "into:") ||
		strings.Contains(opt, `"once":true`) || strings.Contains(opt, `"once": true`)
}
