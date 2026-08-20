package pump

import (
	"fogg/src/pump/want"
	"fogg/src/type"
)

func Impl(address types.Address, payload types.Payload, options types.Options) int {
	if want.None(options) {
		return none(address, payload, options)
	}
	if want.Once(options) {
		return once(address, payload, options)
	}
	if want.Many(options) {
		return many(address, payload, options)
	}
	return -1
}
