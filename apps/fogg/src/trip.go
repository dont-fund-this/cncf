package main

import (
	"fogg/src/pump/into"
	"fogg/src/type"
)

func trip() []types.Trip {
	return []types.Trip{
		{
			Address: into.StringToC("/version"),
			Payload: into.StringToC("{}"),
			Options: into.StringToC("into:some-id"),
		},
		{
			Address: into.StringToC("/storage"),
			Payload: into.StringToC("{}"),
			Options: into.StringToC("into:some-other-id"),
		},
	}
}
