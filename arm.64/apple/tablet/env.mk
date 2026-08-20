ROOT ?= $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../../..)
ROOT := $(abspath $(ROOT))
include $(ROOT)/env.mk
PM := $(dir $(ROOT))_pm_
DEVICE := $(ROOT)/arm.64/apple/tablet
TARGET := alpi
APP_ID := org.dont-fund-this.tablet
DEVICE_ID := apple-tablet-alpi
PROOF := alpi.txt
RAM := 64
FRAME := 2048x1536

BUILD := $(DEVICE)/build
WORK := $(BUILD)/work
RUNTIME := $(BUILD)/runtime
SYS := $(BUILD)/simsys
LIB_DIST := $(BUILD)/libs
DIST := $(ROOT)/dist/arm.64/apple/tablet/$(TARGET)
APP := $(DIST)/alpi.app
FRAMEWORKS := $(APP)/Frameworks
INFO := $(DEVICE)/Info.plist
LIBS := $(DEVICE)/libs
EFS := $(LIBS)/efs
DEPS := $(DEVICE)/deps
OCIS := $(DEVICE)/ocis
GUEST := $(OCIS)/guest
ENGINE := $(OCIS)/engine
OCIS_REL := ocis/riscv64/apple/tablet/$(TARGET)
OCIS_REFS := $(ROOT)/refs/$(OCIS_REL)

SDK := iphonesimulator
TRIPLE := arm64-apple-ios17.0-simulator
RUST_TARGET := aarch64-apple-ios-sim
MIN_OS := 17.0
RUSTLIB ?= $(shell rustc +nightly --print sysroot)/lib/rustlib/src/rust/library
RUSTC = rustc +nightly --target $(RUST_TARGET) -C opt-level=z -C lto -C codegen-units=1 -C panic=abort
SYS_ARGS = -L "$(SYS)" --extern core="$(SYS)/libcore.rlib" --extern alloc="$(SYS)/liballoc.rlib" --extern compiler_builtins="$(SYS)/libcompiler_builtins.rlib"

HOST := apple-tablet-alpi
DEVICE_KIND := $(TARGET)
HOUSE := pat-apple-tablet-alpi
PODMAN := podman --connection $(HOUSE)
RUST_IMAGE := localhost/pat-apple-tablet-alpi-rata:latest
BOOT_IMAGE := localhost/pat-apple-tablet-alpi-boot:latest

SIM := pat-pad
SIM_DEVICE_TYPE := com.apple.CoreSimulator.SimDeviceType.iPad-Pro-11-inch-M4-8GB
SIM_RUNTIME := com.apple.CoreSimulator.SimRuntime.iOS-26-4
SIM_SET := $(BUILD)/simulator/device-set
SIM_STATE := $(BUILD)/simulator/udid

REAL_DEVICE_ID := 856631FE-DAD4-5C0C-8F01-1CBB515A363F

export DEVELOPER_DIR := /Applications/Xcode.app/Contents/Developer
export ROOT PM DEVICE TARGET APP_ID DEVICE_ID PROOF RAM FRAME BUILD WORK RUNTIME SYS LIB_DIST DIST
export APP FRAMEWORKS INFO SDK TRIPLE RUST_TARGET MIN_OS SIM SIM_SET DEVELOPER_DIR
export LIBS EFS DEPS OCIS GUEST ENGINE RUSTLIB RUSTC SYS_ARGS
export HOST DEVICE_KIND HOUSE PODMAN RUST_IMAGE BOOT_IMAGE
