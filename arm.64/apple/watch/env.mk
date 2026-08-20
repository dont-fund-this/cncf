ROOT ?= $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../../..)
ROOT := $(abspath $(ROOT))
include $(ROOT)/env.mk
PM := $(dir $(ROOT))_pm_
DEVICE := $(ROOT)/arm.64/apple/watch
TARGET := alpi
APP_ID := dev.pat.ta-in-patr-watch-alpi
DEVICE_ID := apple-watch-alpi
PROOF := alpi.txt
RAM := 16
FRAME := 396x484

BUILD ?= $(DEVICE)/build
WORK ?= $(BUILD)/work
RUNTIME ?= $(BUILD)/runtime
SYS ?= $(BUILD)/simsys
LIB_DIST ?= $(BUILD)/libs
DIST ?= $(ROOT)/dist/arm.64/apple/watch/$(TARGET)
APP ?= $(DIST)/$(TARGET).app
FRAMEWORKS ?= $(APP)/Frameworks
LIBS ?= $(DEVICE)/libs
EFS ?= $(LIBS)/efs
DEPS ?= $(DEVICE)/deps
OCIS ?= $(DEVICE)/ocis
ENGINE ?= $(OCIS)/engine
GUEST ?= $(OCIS)/guest
INFO ?= $(DEVICE)/Info.plist

OCIS_REL ?= ocis/riscv64/apple/watch/$(TARGET)
OCIS_REFS ?= $(ROOT)/refs/$(OCIS_REL)
CODE_ZIP ?= $(ROOT)/refs/code/code.zip
BIN ?= $(BUILD)/libefs.bin
ZIP ?= $(BUILD)/code.zip

SDK ?= watchsimulator
TRIPLE ?= arm64-apple-watchos10.0-simulator
RUST_TARGET ?= aarch64-apple-watchos-sim
MIN_OS ?= 10.0
RUSTLIB ?= $(shell rustc +nightly --print sysroot)/lib/rustlib/src/rust/library
RUSTC = rustc +nightly --target $(RUST_TARGET) -C opt-level=z -C lto -C codegen-units=1 -C panic=abort
SYS_ARGS = -L "$(SYS)" --extern core="$(SYS)/libcore.rlib" --extern alloc="$(SYS)/liballoc.rlib" --extern compiler_builtins="$(SYS)/libcompiler_builtins.rlib"

HOST := apple-watch-alpi
DEVICE_KIND := $(TARGET)
HOUSE := pat-apple-watch-alpi
PODMAN := podman --connection $(HOUSE)
RUST_IMAGE := localhost/pat-apple-watch-alpi-rata:latest
BOOT_IMAGE := localhost/pat-apple-watch-alpi-boot:latest
SIM := pat-alpi-cncf-watch
SIM_DEVICE_TYPE := com.apple.CoreSimulator.SimDeviceType.Apple-Watch-Series-11-46mm
SIM_RUNTIME := com.apple.CoreSimulator.SimRuntime.watchOS-26-4
SIM_SET := $(BUILD)/simulator/device-set
SIM_STATE := $(BUILD)/simulator/udid
SIM_PROOF := $(BUILD)/proof

override TMPDIR := $(BUILD)/tooling/tmp
override DEVELOPER_DIR := /Applications/Xcode.app/Contents/Developer
export ROOT PM DEVICE TARGET APP_ID DEVICE_ID PROOF RAM FRAME BUILD WORK RUNTIME SYS LIB_DIST DIST
export APP FRAMEWORKS LIBS EFS DEPS OCIS ENGINE GUEST INFO OCIS_REL OCIS_REFS CODE_ZIP BIN ZIP
export SDK TRIPLE RUST_TARGET MIN_OS RUSTLIB RUSTC SYS_ARGS HOST DEVICE_KIND RAM FRAME HOUSE PODMAN RUST_IMAGE BOOT_IMAGE SIM SIM_SET
export SIM_STATE SIM_PROOF TMPDIR DEVELOPER_DIR
export SIM_DEVICE_TYPE SIM_RUNTIME

export DEVICE
