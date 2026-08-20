use crate::r#type::Trip;

pub fn trip() -> [Trip; 2] {
    [
        Trip {
            address: c"/version".as_ptr(),
            payload: cr#"{}"#.as_ptr(),
            options: c"into:some-id".as_ptr(),
        },
        Trip {
            address: c"/help".as_ptr(),
            payload: cr#"{}"#.as_ptr(),
            options: c"into:some-other-id".as_ptr(),
        },
    ]
}
