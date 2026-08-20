module main

import os
import dl

pub fn bind(binary_path string) ?Cabi {
	filename := os.file_name(binary_path)
	skips := ['c', 'cpp', 'rust', 'go', 'swift', 'haskell', 'zig', 'v', 'slint_sample']
	if filename in skips {
		return none
	}

	handle := dl.open(binary_path, dl.rtld_lazy | dl.rtld_local)
	if handle == 0 {
		return none
	}

	pump_sym := dl.sym(handle, 'Pump')
	if pump_sym == 0 {
		dl.close(handle)
		return none
	}

	return Cabi{
		name: filename
		path: binary_path
		pump: unsafe { PumpFn(pump_sym) }
	}
}
