package main

import (
	"os"
	"path/filepath"
)

func find(targetDir string) []string {
	dir := targetDir
	if dir == "" {
		if envDir := os.Getenv("DIST_DIR"); envDir != "" {
			dir = envDir
		} else {
			candidates := []string{
				"dist",
				"../../dist",
				"../../../dist",
			}
			for _, c := range candidates {
				if fi, err := os.Stat(c); err == nil && fi.IsDir() {
					dir = c
					break
				}
			}
		}
	}
	if dir == "" {
		dir = "dist"
	}

	entries, err := os.ReadDir(dir)
	if err != nil {
		return nil
	}

	var files []string
	for _, entry := range entries {
		if !entry.IsDir() && entry.Name() != ".DS_Store" {
			files = append(files, filepath.Join(dir, entry.Name()))
		}
	}
	return files
}
