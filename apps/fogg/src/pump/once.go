package pump

import "fogg/src/type"

func once(address types.Address, payload types.Payload, options types.Options) int {
	if WithFn == nil {
		return -1
	}
	for _, def := range WithFn() {
		if def.Fit(address, payload, options) {
			return def.Fun(address, payload, options)
		}
	}
	return -1
}
