package json

func JsonParseBool(s string) (bool, bool) {
	if s == "true" {
		return true, true
	}
	if s == "false" {
		return false, true
	}
	return false, false
}
