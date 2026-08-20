package main

import "os"

func boot(targetDir string) []*Cabi {
	var engines []*Cabi
	if envLib := os.Getenv("PAT_LIB"); envLib != "" {
		if c := bind(envLib); c != nil {
			engines = append(engines, c)
			return engines
		}
	}

	files := find(targetDir)
	for _, file := range files {
		if c := bind(file); c != nil {
			engines = append(engines, c)
		}
	}
	return engines
}
