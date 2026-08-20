package pump

import "fogg/src/type"

func none(address types.Address, payload types.Payload, options types.Options) int {
	_ = address
	_ = payload
	_ = options
	return -1
}
