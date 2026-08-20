mod bind;
mod boot;
mod find;
mod trip;
mod r#type;

use std::env;
use std::ffi::CString;

fn main() {
    let args: Vec<String> = env::args().collect();
    let target_dir = args.get(1).map(|s| s.as_str());

    let dist = boot::boot(target_dir);
    if !dist.is_empty() {
        let trips = trip::trip();
        for d in &dist {
            for t in &trips {
                let addr = CString::new(t.address).unwrap();
                let pay = CString::new(t.payload).unwrap();
                let opt = CString::new(t.options).unwrap();
                unsafe {
                    (d.pump)(addr.as_ptr(), pay.as_ptr(), opt.as_ptr());
                }
            }
        }
    }

    println!("{{\n  \"lang\": \"rust\",\n  \"status\": \"ready\",\n  \"engines\": {}\n}}", dist.len());
}
