package main

import (
	"fmt"
	"fogg/src/defs"
	"fogg/src/pump"
	"fogg/src/pump/into"
	"fogg/src/type"
)

func main() {
	someDef := types.Def{
		Sid: into.StringToC("some-id"),
		Tag: into.StringToC("thing1"),
		Fit: func(address types.Address, payload types.Payload, options types.Options) bool {
			if address == nil {
				return false
			}
			addr := into.CStr(address)
			return addr == "some-id"
		},
		Fun: func(address types.Address, payload types.Payload, options types.Options) int {
			if payload != nil {
				fmt.Println(into.CStr(payload))
			}
			return 0
		},
	}

	someOtherDef := types.Def{
		Sid: into.StringToC("some-other-id"),
		Tag: into.StringToC("thing2"),
		Fit: func(address types.Address, payload types.Payload, options types.Options) bool {
			return address != nil && into.CStr(address) == "some-other-id"
		},
		Fun: func(address types.Address, payload types.Payload, options types.Options) int {
			if payload != nil {
				fmt.Println(into.CStr(payload))
			}
			return 0
		},
	}

	defs.More(someDef)
	defs.More(someOtherDef)

	for _, t := range trip() {
		pump.Pump(t.Address, t.Payload, t.Options)
	}

	defs.Less(someDef)
	defs.Less(someOtherDef)
}
