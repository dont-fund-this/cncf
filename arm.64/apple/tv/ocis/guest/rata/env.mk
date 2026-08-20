include $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../env.mk)

GUEST_RATA := $(GUEST)/rata
GUEST_SYSROOT := $(GUEST)/build/sysroot
GUEST_RATA_BIN := $(GUEST_RATA)/build/rata
RATA_BUILD := $(GUEST_RATA)/build
RATA_INIT := $(RATA_BUILD)/init
RATA_INIT_SOURCE := $(RATA_INIT)/source
RATA_RATATUI_SOURCE := $(DEPS)/ratatui/source
RATA_IMAGE := $(HOUSE)-guest-rata
RATA_SOURCE := $(wildcard $(GUEST_RATA)/src/*.rs $(GUEST_RATA)/src/**/*.rs)

export GUEST_RATA GUEST_SYSROOT GUEST_RATA_BIN
export RATA_BUILD RATA_INIT RATA_INIT_SOURCE
export RATA_RATATUI_SOURCE RATA_IMAGE RATA_SOURCE
