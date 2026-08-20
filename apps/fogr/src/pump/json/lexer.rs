pub fn skip_whitespace(s: &[u8], mut i: usize) -> usize {
    while i < s.len() && (s[i] == b' ' || s[i] == b'\t' || s[i] == b'\n' || s[i] == b'\r') {
        i += 1;
    }
    i
}

pub fn skip_string(s: &[u8], mut i: usize) -> usize {
    if i >= s.len() || s[i] != b'"' {
        return i;
    }
    i += 1;
    while i < s.len() {
        if s[i] == b'\\' {
            i += 2;
        } else if s[i] == b'"' {
            return i + 1;
        } else {
            i += 1;
        }
    }
    i
}

pub fn skip_value(s: &[u8], mut i: usize) -> usize {
    i = skip_whitespace(s, i);
    if i >= s.len() {
        return i;
    }
    if s[i] == b'"' {
        return skip_string(s, i);
    }
    if s[i] == b'{' {
        let mut depth = 1;
        i += 1;
        while i < s.len() && depth > 0 {
            if s[i] == b'"' {
                i = skip_string(s, i);
            } else {
                if s[i] == b'{' {
                    depth += 1;
                } else if s[i] == b'}' {
                    depth -= 1;
                }
                i += 1;
            }
        }
        return i;
    }
    if s[i] == b'[' {
        let mut depth = 1;
        i += 1;
        while i < s.len() && depth > 0 {
            if s[i] == b'"' {
                i = skip_string(s, i);
            } else {
                if s[i] == b'[' {
                    depth += 1;
                } else if s[i] == b']' {
                    depth -= 1;
                }
                i += 1;
            }
        }
        return i;
    }
    while i < s.len()
        && s[i] != b','
        && s[i] != b'}'
        && s[i] != b']'
        && s[i] != b' '
        && s[i] != b'\n'
        && s[i] != b'\r'
        && s[i] != b'\t'
    {
        i += 1;
    }
    i
}

pub fn find_field<'a>(s: &'a [u8], key: &[u8]) -> Option<&'a [u8]> {
    let mut i = skip_whitespace(s, 0);
    if i >= s.len() || s[i] != b'{' {
        return None;
    }
    i += 1;

    while i < s.len() {
        i = skip_whitespace(s, i);
        if i >= s.len() || s[i] == b'}' {
            break;
        }

        if s[i] != b'"' {
            break;
        }
        let kstart = i + 1;
        i = skip_string(s, i);
        let kend = if i > 0 && s[i - 1] == b'"' { i - 1 } else { i };

        i = skip_whitespace(s, i);
        if i >= s.len() || s[i] != b':' {
            break;
        }
        i += 1;
        i = skip_whitespace(s, i);

        let vstart = i;
        let vend = skip_value(s, i);

        if &s[kstart..kend] == key {
            if vstart < s.len() && s[vstart] == b'"' && vend > vstart && s[vend - 1] == b'"' {
                return Some(&s[vstart + 1..vend - 1]);
            }
            return Some(&s[vstart..vend]);
        }

        i = vend;
        i = skip_whitespace(s, i);
        if i < s.len() && s[i] == b',' {
            i += 1;
        }
    }
    None
}
