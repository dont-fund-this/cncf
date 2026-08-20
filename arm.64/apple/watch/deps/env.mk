include $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../env.mk)

ALPINE := $(DEPS)/alpine
DISKIMAGE := $(DEPS)/diskimage
GENEXT2FS_DEP := $(DEPS)/genext2fs
RATATUI := $(DEPS)/ratatui
TINYEMU := $(DEPS)/tinyemu
