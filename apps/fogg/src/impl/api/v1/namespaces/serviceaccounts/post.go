package serviceaccounts

import (
	"encoding/hex"
	"fmt"
	"fogg/src/pump"
	"fogg/src/pump/into"
	"fogg/src/pump/json"
	"fogg/src/type"
	"strings"
)

func extractName(payload types.Payload) string {
	if payload == nil {
		return ""
	}
	s := into.CStr(payload)

	// 1. Direct JSON "name" field extraction
	if val, ok := json.FindField(s, "name"); ok {
		return val
	}

	// 2. Wire hex extraction (protobuf)
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

func extractNamespace(addr string) (string, bool) {
	prefix := "/api/v1/namespaces/"
	relPrefix := "namespaces/"

	var rest string
	if strings.HasPrefix(addr, prefix) {
		rest = addr[len(prefix):]
	} else if strings.HasPrefix(addr, relPrefix) {
		rest = addr[len(relPrefix):]
	} else {
		return "", false
	}

	slash := strings.Index(rest, "/")
	if slash == -1 {
		return "", false
	}

	ns := rest[:slash]
	after := rest[slash:]

	if after == "/serviceaccounts" && ns != "" {
		return ns, true
	}
	return "", false
}

var ServiceAccountPost types.Def

func init() {
	ServiceAccountPost = types.Def{
		Sid: into.StringToC("/api/v1/namespaces/serviceaccounts"),
		Tag: into.StringToC("/api/v1/namespaces/serviceaccounts"),
		Fit: func(address types.Address, payload types.Payload, options types.Options) bool {
			if address == nil {
				return false
			}
			addr := into.CStr(address)
			_, ok := extractNamespace(addr)
			return ok && into.HasVerb(options, "POST")
		},
		Fun: func(address types.Address, payload types.Payload, options types.Options) int {
			if !ServiceAccountPost.Fit(address, payload, options) {
				return -1
			}
			addr := into.CStr(address)
			ns, ok := extractNamespace(addr)
			if !ok {
				return -1
			}
			sa := extractName(payload)
			if sa == "" {
				return -1
			}
			res := fmt.Sprintf(`{"apiVersion":"v1","kind":"ServiceAccount","metadata":{"name":"%s","namespace":"%s","uid":"1","resourceVersion":"1"}}`, sa, ns)
			target := into.GetInto(options)
			if target != nil {
				return pump.Pump(target, into.StringToC(res), into.StringToC("once"))
			}
			return 1
		},
	}
}
