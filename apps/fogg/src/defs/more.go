package defs

import "fogg/src/type"

func More(def types.Def) int {
	load()
	insertIdx := len(build)
	defsList = append(defsList[:insertIdx], append([]types.Def{def}, defsList[insertIdx:]...)...)
	return 0
}
