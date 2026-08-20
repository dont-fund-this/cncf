include $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../env.mk)

GENEXT2FS := $(OCIS)/genext2fs
GUEST := $(OCIS)/guest
ENGINE := $(OCIS)/engine

export GENEXT2FS GUEST ENGINE
