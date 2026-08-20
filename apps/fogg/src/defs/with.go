package defs

import (
	"fogg/src/impl"
	"fogg/src/pump"
	"fogg/src/type"
)

var build types.Defs
var defsList types.Defs
var did bool

func init() {
	pump.WithFn = With
	build = impl.All()
}

func load() {
	if !did {
		defsList = append([]types.Def{}, build...)
		did = true
	}
}

func With() types.Defs {
	load()
	return defsList
}
