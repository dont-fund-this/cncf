package pump

import "fogg/src/type"

func many(address types.Address, payload types.Payload, options types.Options) int {
	if WithFn == nil {
		return 0
	}
	count := 0
	for _, def := range WithFn() {
		if def.Fit(address, payload, options) {
			def.Fun(address, payload, options)
			count++
		}
	}
	return count
}
