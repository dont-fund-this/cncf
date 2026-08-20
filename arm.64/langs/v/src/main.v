module main

import os

fn main() {
	target_dir := if os.args.len > 1 { os.args[1] } else { '' }
	dist := boot(if target_dir != '' { target_dir } else { none })

	if dist.len > 0 {
		trips := trip()
		for d in dist {
			if p := d.pump {
				for t in trips {
					p(t.address.str, t.payload.str, t.options.str)
				}
			}
		}
	}

	println('{\n  "lang": "v",\n  "status": "ready",\n  "engines": ${dist.len}\n}')
}
