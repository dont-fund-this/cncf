package defs

import (
	"fogg/src/pump/into"
	"fogg/src/type"
)

func Less(def types.Def) int {
	load()
	sid := into.CStr(def.Sid)
	tag := into.CStr(def.Tag)
	for i := len(build); i < len(defsList); i++ {
		scanSid := into.CStr(defsList[i].Sid)
		scanTag := into.CStr(defsList[i].Tag)
		if (sid != "" && scanSid == sid) || (tag != "" && scanTag == tag) {
			defsList = append(defsList[:i], defsList[i+1:]...)
			return 0
		}
	}
	return -1
}
