use crate::r#type::Triplet;

pub fn trip() -> Vec<Triplet> {
    vec![
        Triplet {
            address: "/version",
            payload: "{}",
            options: "{\"once\":true}",
        },
        Triplet {
            address: "/storage",
            payload: "{}",
            options: "{\"once\":true}",
        },
        Triplet {
            address: "sql.help",
            payload: "{}",
            options: "{\"once\":true}",
        },
    ]
}
