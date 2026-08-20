package version

import (
	_ "embed"
	"fogg/src/pump"
	"fogg/src/pump/into"
	"fogg/src/type"
)

//go:embed get.json
var versionJSON string

var VersionGet types.Def

func init() {
	VersionGet = types.Def{
		Sid: into.StringToC("version"),
		Tag: into.StringToC("tag,any"),
		Fit: func(address types.Address, payload types.Payload, options types.Options) bool {
			if address == nil {
				return false
			}
			addr := into.CStr(address)
			return (addr == "/version" || addr == "version") && into.HasVerb(options, "GET")
		},
		Fun: func(address types.Address, payload types.Payload, options types.Options) int {
			if !VersionGet.Fit(address, payload, options) {
				return -1
			}
			target := into.GetInto(options)
			if target != nil {
				return pump.Pump(target, into.StringToC(versionJSON), into.StringToC("once"))
			}
			return 1
		},
	}
}
