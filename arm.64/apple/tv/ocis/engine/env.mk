include $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../env.mk)

ENGINE_BUILD := $(ENGINE)/build
GUEST_OUT := $(GUEST)/build/out
TINYEMU := $(DEPS)/tinyemu
BOOT_IMAGE := $(HOUSE)-boot
BOOT_SECS ?= 25
FRAME_W := $(word 1,$(subst x, ,$(FRAME)))
FRAME_H := $(word 2,$(subst x, ,$(FRAME)))
