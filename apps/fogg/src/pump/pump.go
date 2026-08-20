package pump

import "fogg/src/type"

var WithFn func() types.Defs

func Pump(address types.Address, payload types.Payload, options types.Options) int {
	return Impl(address, payload, options)
}
