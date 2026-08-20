use crate::pump::into::{get_into, has_verb};
use crate::pump::json::find_field;
use crate::pump::Pump;
use crate::r#type::{Address, Def, Options, Payload};
use core::ffi::CStr;

fn hex_val(b: u8) -> Option<u8> {
    if b >= b'0' && b <= b'9' {
        Some(b - b'0')
    } else if b >= b'a' && b <= b'f' {
        Some(b - b'a' + 10)
    } else if b >= b'A' && b <= b'F' {
        Some(b - b'A' + 10)
    } else {
        None
    }
}

fn extract_name(payload: *const core::ffi::c_char, out: &mut [u8]) -> usize {
    if payload.is_null() {
        return 0;
    }
    let s = unsafe { CStr::from_ptr(payload).to_bytes() };

    // 1. Direct JSON "name" field extraction via pump/json parser
    if let Some(val) = find_field(s, b"name") {
        let len = val.len().min(out.len());
        out[..len].copy_from_slice(&val[..len]);
        return len;
    }

    // 2. Wire hex extraction via pump/json parser
    if let Some(hex_slice) = find_field(s, b"hex") {
        let mut raw = [0u8; 256];
        let mut raw_len = 0;
        let mut idx = 0;
        while idx + 1 < hex_slice.len() && raw_len < raw.len() {
            if let (Some(h), Some(l)) = (hex_val(hex_slice[idx]), hex_val(hex_slice[idx + 1])) {
                raw[raw_len] = (h << 4) | l;
                raw_len += 1;
            }
            idx += 2;
        }

        if raw_len > 20 && raw[0] == b'k' && raw[1] == b'8' && raw[2] == b's' && raw[3] == 0 {
            let mut i = 20;
            while i + 2 < raw_len {
                if raw[i] == 0x0a {
                    let len = raw[i + 1] as usize;
                    if len > 0 && len <= 63 && i + 2 + len <= raw_len {
                        let mut valid = true;
                        let mut k = 0;
                        while k < len {
                            let c = raw[i + 2 + k];
                            if !((c >= b'a' && c <= b'z') || (c >= b'0' && c <= b'9') || c == b'-')
                            {
                                valid = false;
                                break;
                            }
                            k += 1;
                        }
                        if valid {
                            let clen = len.min(out.len());
                            let mut j = 0;
                            while j < clen {
                                out[j] = raw[i + 2 + j];
                                j += 1;
                            }
                            return clen;
                        }
                    }
                }
                i += 1;
            }
        }
    }

    0
}

pub static NamespacePost: Def = Def {
    sid: b"/api/v1/namespaces\0".as_ptr() as *const i8,
    tag: b"/api/v1/namespaces\0".as_ptr() as *const i8,
    fit: |address: Address, _p: Payload, options: Options| -> bool {
        if address.is_null() {
            return false;
        }
        let addr = unsafe { CStr::from_ptr(address).to_bytes() };
        (addr == b"/api/v1/namespaces" || addr == b"namespaces") && has_verb(options, b"POST")
    },
    fun: |address: Address, payload: Payload, options: Options| -> i32 {
        if !(NamespacePost.fit)(address, payload, options) {
            return -1;
        }
        let mut name = [0u8; 64];
        let nlen = extract_name(payload, &mut name);
        if nlen == 0 {
            return -1;
        }
        let ns = &name[..nlen];

        let mut buf = [0u8; 256];
        let p1 = b"{\"apiVersion\":\"v1\",\"kind\":\"Namespace\",\"metadata\":{\"name\":\"";
        let p2 =
            b"\",\"uid\":\"1\",\"resourceVersion\":\"1\"},\"status\":{\"phase\":\"Active\"}}\0";
        let mut idx = 0;
        buf[idx..idx + p1.len()].copy_from_slice(p1);
        idx += p1.len();
        buf[idx..idx + ns.len()].copy_from_slice(ns);
        idx += ns.len();
        buf[idx..idx + p2.len()].copy_from_slice(p2);

        if let Some(into_target) = get_into(options) {
            return Pump(
                into_target,
                buf.as_ptr() as *const i8,
                b"once\0".as_ptr() as *const i8,
            );
        }
        1
    },
};
