module main

import os

pub fn find(target_dir ?string) []string {
	mut dir := ''
	if td := target_dir {
		dir = td
	} else if env_dir := os.getenv_opt('DIST_DIR') {
		dir = env_dir
	} else {
		if os.is_dir('../../../dist') {
			dir = '../../../dist'
		} else if os.is_dir('dist') {
			dir = 'dist'
		} else if os.is_dir('../../dist') {
			dir = '../../dist'
		} else {
			dir = 'dist'
		}
	}

	if !os.is_dir(dir) {
		return []
	}

	mut files := []string{}
	entries := os.ls(dir) or { return [] }
	for entry in entries {
		if entry != '.DS_Store' {
			p := os.join_path(dir, entry)
			if !os.is_dir(p) {
				files << p
			}
		}
	}
	return files
}
