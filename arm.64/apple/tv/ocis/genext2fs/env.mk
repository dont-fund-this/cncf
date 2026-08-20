include $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../env.mk)

GENEXT2FS := $(OCIS)/genext2fs
DEP := $(DEPS)/genext2fs
ENGINE := $(OCIS)/engine
BUILD := $(GENEXT2FS)/build
DIST := $(GENEXT2FS)/dist/bin
BIN := $(DIST)/genext2fs
SOURCE := $(DEP)/source/genext2fs-v1.6.2.tar.gz
CONTAINERFILE := $(GENEXT2FS)/Containerfile
IMAGE := $(HOUSE)-genext2fs
PODMAN := podman --connection $(HOUSE)

export GENEXT2FS DEP ENGINE BUILD DIST BIN SOURCE CONTAINERFILE IMAGE PODMAN
