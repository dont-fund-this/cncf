module main

import os

pub fn boot(target_dir ?string) []Cabi {
	mut engines := []Cabi{}
	if env_lib := os.getenv_opt('PAT_LIB') {
		if env_lib != '' {
			if c := bind(env_lib) {
				engines << c
				return engines
			}
		}
	}

	files := find(target_dir)
	for file in files {
		if c := bind(file) {
			engines << c
		}
	}
	return engines
}
