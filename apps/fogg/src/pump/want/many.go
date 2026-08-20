package want

import (
	"fogg/src/pump/into"
	"fogg/src/type"
)

func Many(options types.Options) bool {
	if options == nil {
		return false
	}
	opt := into.CStr(options)
	return opt == "many"
}
