package json

func SkipWhitespace(s string, i int) int {
	for i < len(s) && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r') {
		i++
	}
	return i
}

func SkipString(s string, i int) int {
	if i >= len(s) || s[i] != '"' {
		return i
	}
	i++
	for i < len(s) {
		if s[i] == '\\' {
			i += 2
		} else if s[i] == '"' {
			return i + 1
		} else {
			i++
		}
	}
	return i
}

func SkipValue(s string, i int) int {
	i = SkipWhitespace(s, i)
	if i >= len(s) {
		return i
	}
	if s[i] == '"' {
		return SkipString(s, i)
	}
	if s[i] == '{' {
		depth := 1
		i++
		for i < len(s) && depth > 0 {
			if s[i] == '"' {
				i = SkipString(s, i)
			} else {
				if s[i] == '{' {
					depth++
				} else if s[i] == '}' {
					depth--
				}
				i++
			}
		}
		return i
	}
	if s[i] == '[' {
		depth := 1
		i++
		for i < len(s) && depth > 0 {
			if s[i] == '"' {
				i = SkipString(s, i)
			} else {
				if s[i] == '[' {
					depth++
				} else if s[i] == ']' {
					depth--
				}
				i++
			}
		}
		return i
	}
	for i < len(s) && s[i] != ',' && s[i] != '}' && s[i] != ']' && s[i] != ' ' && s[i] != '\n' && s[i] != '\r' && s[i] != '\t' {
		i++
	}
	return i
}

func FindField(s string, key string) (string, bool) {
	i := SkipWhitespace(s, 0)
	if i >= len(s) || s[i] != '{' {
		return "", false
	}
	i++

	for i < len(s) {
		i = SkipWhitespace(s, i)
		if i >= len(s) || s[i] == '}' {
			break
		}
		if s[i] != '"' {
			break
		}

		kstart := i + 1
		i = SkipString(s, i)
		kend := i
		if i > 0 && s[i-1] == '"' {
			kend = i - 1
		}

		i = SkipWhitespace(s, i)
		if i >= len(s) || s[i] != ':' {
			break
		}
		i++
		i = SkipWhitespace(s, i)

		vstart := i
		vend := SkipValue(s, i)

		if s[kstart:kend] == key {
			if vstart < len(s) && s[vstart] == '"' && vend > vstart && s[vend-1] == '"' {
				return s[vstart+1 : vend-1], true
			}
			return s[vstart:vend], true
		}

		i = vend
		i = SkipWhitespace(s, i)
		if i < len(s) && s[i] == ',' {
			i++
		}
	}
	return "", false
}
