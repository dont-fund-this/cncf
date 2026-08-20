package namespaces

import (
	"encoding/hex"
	"fmt"
	"fogg/src/pump"
	"fogg/src/pump/into"
	"fogg/src/pump/json"
	"fogg/src/type"
)

func extractName(payload types.Payload) string {
	if payload == nil {
		return ""
	}
	s := into.CStr(payload)

	// 1. Direct JSON "name" field extraction via json.FindField
	if val, ok := json.FindField(s, "name"); ok {
		return val
	}

	// 2. Wire hex extraction via json.FindField
	if hexStr, ok := json.FindField(s, "hex"); ok {
		raw, err := hex.DecodeString(hexStr)
		if err == nil && len(raw) > 20 && string(raw[:4]) == "k8s\x00" {
			for i := 20; i+2 < len(raw); i++ {
				if raw[i] == 0x0a {
					l := int(raw[i+1])
					if l > 0 && l <= 63 && i+2+l <= len(raw) {
						candidate := string(raw[i+2 : i+2+l])
						valid := true
						for _, c := range candidate {
							if !(c >= 'a' && c <= 'z' || c >= '0' && c <= '9' || c == '-') {
								valid = false
								break
							}
						}
						if valid {
							return candidate
						}
					}
				}
			}
		}
	}

	return ""
}

var NamespacePost types.Def

func init() {
	NamespacePost = types.Def{
		Sid: into.StringToC("/api/v1/namespaces"),
		Tag: into.StringToC("/api/v1/namespaces"),
		Fit: func(address types.Address, payload types.Payload, options types.Options) bool {
			if address == nil {
				return false
			}
			addr := into.CStr(address)
			return (addr == "/api/v1/namespaces" || addr == "namespaces") && into.HasVerb(options, "POST")
		},
		Fun: func(address types.Address, payload types.Payload, options types.Options) int {
			if !NamespacePost.Fit(address, payload, options) {
				return -1
			}
			ns := extractName(payload)
			if ns == "" {
				return -1
			}
			res := fmt.Sprintf(`{"apiVersion":"v1","kind":"Namespace","metadata":{"name":"%s","uid":"1","resourceVersion":"1"},"status":{"phase":"Active"}}`, ns)
			target := into.GetInto(options)
			if target != nil {
				return pump.Pump(target, into.StringToC(res), into.StringToC("once"))
			}
			return 1
		},
	}
}
