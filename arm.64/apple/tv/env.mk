ROOT ?= $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../../..)
ROOT := $(abspath $(ROOT))
include $(ROOT)/env.mk
PM := $(dir $(ROOT))_pm_
DEVICE := $(ROOT)/arm.64/apple/tv
TARGET := alpi
APP_ID := org.dont-fund-this.tv
DEVICE_ID := apple-tv-alpi
PROOF := alpi.txt
RAM := 64
FRAME := 1920x1080

BUILD := $(DEVICE)/build
WORK := $(BUILD)/work
RUNTIME := $(BUILD)/runtime
SYS := $(BUILD)/simsys
LIB_DIST := $(BUILD)/libs
DIST := $(ROOT)/dist/arm.64/apple/tv/$(TARGET)
APP := $(DIST)/alpi.app
FRAMEWORKS := $(APP)/Frameworks
INFO := $(DEVICE)/Info.plist
LIBS := $(DEVICE)/libs
EFS := $(LIBS)/efs
DEPS := $(DEVICE)/deps
OCIS := $(DEVICE)/ocis
GUEST := $(OCIS)/guest
ENGINE := $(OCIS)/engine
OCIS_REL := ocis/riscv64/apple/tv/$(TARGET)
OCIS_REFS := $(ROOT)/refs/$(OCIS_REL)

SDK := appletvsimulator
TRIPLE := arm64-apple-tvos17.0-simulator
RUST_TARGET := aarch64-apple-tvos-sim
MIN_OS := 17.0
RUSTLIB ?= $(shell rustc +nightly --print sysroot)/lib/rustlib/src/rust/library
RUSTC = rustc +nightly --target $(RUST_TARGET) -C opt-level=z -C lto -C codegen-units=1 -C panic=abort
SYS_ARGS = -L "$(SYS)" --extern core="$(SYS)/libcore.rlib" --extern alloc="$(SYS)/liballoc.rlib" --extern compiler_builtins="$(SYS)/libcompiler_builtins.rlib"

HOST := apple-tv-alpi
DEVICE_KIND := $(TARGET)
HOUSE := pat-apple-tv-alpi
PODMAN := podman --connection $(HOUSE)
RUST_IMAGE := localhost/pat-apple-tv-alpi-rata:latest
BOOT_IMAGE := localhost/pat-apple-tv-alpi-boot:latest

SIM := pat-tv
SIM_DEVICE_TYPE := com.apple.CoreSimulator.SimDeviceType.Apple-TV-4K-3rd-generation-4K
SIM_RUNTIME := com.apple.CoreSimulator.SimRuntime.tvOS-26-4
SIM_SET := $(BUILD)/simulator/device-set
SIM_STATE := $(BUILD)/simulator/udid

export DEVELOPER_DIR := /Applications/Xcode.app/Contents/Developer
export ROOT PM DEVICE TARGET APP_ID DEVICE_ID PROOF RAM FRAME BUILD WORK RUNTIME SYS LIB_DIST DIST
export APP FRAMEWORKS INFO SDK TRIPLE RUST_TARGET MIN_OS SIM SIM_SET DEVELOPER_DIR
export LIBS EFS DEPS OCIS GUEST ENGINE RUSTLIB RUSTC SYS_ARGS
export HOST DEVICE_KIND HOUSE PODMAN RUST_IMAGE BOOT_IMAGE
