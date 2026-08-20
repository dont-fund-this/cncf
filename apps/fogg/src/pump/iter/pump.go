package iter

import (
	"fogg/src/pump"
	"fogg/src/type"
)

func PumpIter(address types.Address, items []types.Payload, options types.Options) int {
	if address == nil {
		return -1
	}
	for _, it := range items {
		if it != nil {
			pump.Pump(address, it, options)
		}
	}
	return len(items)
}
