include $(CURDIR)/env.mk

SRC := $(wildcard $(DEVICE)/src/*.swift) $(wildcard $(DEVICE)/src/find/*.swift) $(wildcard $(DEVICE)/src/with/*.swift) $(wildcard $(DEVICE)/src/into/*.swift)
APP_BIN := $(APP)/pat

.DEFAULT_GOAL := test
.NOTPARALLEL:

.PHONY: test tidy burn

test:
	@mkdir -p "$(TMPDIR)"
	@$(MAKE) -C "$(DEPS)" test
	@$(MAKE) -C "$(LIBS)" test
	@$(MAKE) -C "$(OCIS)" test
	@mkdir -p "$(APP)" "$(FRAMEWORKS)"
	@for lib in box efs jam tui; do \
	  if [ -f "$(BUILD)/libs/lib$$lib" ]; then \
	    mkdir -p "$(FRAMEWORKS)/lib$$lib.framework"; \
	    cp "$(BUILD)/libs/lib$$lib" "$(FRAMEWORKS)/lib$$lib.framework/lib$$lib"; \
	    printf '<?xml version="1.0" encoding="UTF-8"?>\n<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">\n<plist version="1.0">\n<dict>\n    <key>CFBundleIdentifier</key>\n    <string>dev.pat.lib%s</string>\n    <key>CFBundleExecutable</key>\n    <string>lib%s</string>\n    <key>CFBundlePackageType</key>\n    <string>FMWK</string>\n    <key>CFBundleShortVersionString</key>\n    <string>1.0</string>\n    <key>CFBundleVersion</key>\n    <string>1</string>\n    <key>MinimumOSVersion</key>\n    <string>10.0</string>\n</dict>\n</plist>\n' "$$lib" "$$lib" > "$(FRAMEWORKS)/lib$$lib.framework/Info.plist"; \
	    codesign --force --sign - "$(FRAMEWORKS)/lib$$lib.framework"; \
	  fi; \
	done
	@xcrun -sdk "$(SDK)" swiftc -parse-as-library -target "$(TRIPLE)" \
	  -import-objc-header "$(DEVICE)/src/bridge.h" \
	  -Xlinker -rpath -Xlinker @executable_path/Frameworks \
	  $(SRC) \
	  -o "$(APP_BIN)"
	@cp "$(INFO)" "$(APP)/Info.plist"
	@cp "$(OCIS)/guest/build/out/bios64.bin" "$(APP)/bios64.bin"
	@cp "$(OCIS)/guest/build/out/risc64.bin" "$(APP)/risc64.bin"
	@cp "$(OCIS)/guest/build/out/rootfs.ext2" "$(APP)/rootfs.ext2"
	@codesign --force --sign - "$(APP)"
	@mkdir -p "$(SIM_SET)" "$(dir $(SIM_STATE))"
	@if [ ! -s "$(SIM_STATE)" ]; then \
	  xcrun simctl --set "$(SIM_SET)" create "$(SIM)" "$(SIM_DEVICE_TYPE)" "$(SIM_RUNTIME)" > "$(SIM_STATE)"; \
	fi
	@xcrun simctl --set "$(SIM_SET)" boot "$$(cat "$(SIM_STATE)")" >/dev/null 2>&1 || true
	@xcrun simctl --set "$(SIM_SET)" bootstatus "$$(cat "$(SIM_STATE)")" -b
	@xcrun simctl --set "$(SIM_SET)" install "$$(cat "$(SIM_STATE)")" "$(APP)" 2>/dev/null || true
	@xcrun simctl --set "$(SIM_SET)" launch "$$(cat "$(SIM_STATE)")" "$(APP_ID)" 2>/dev/null || true
	@xcrun simctl install 9C79A0B2-4025-4A4B-BECE-BB98F656118D "$(APP)" 2>/dev/null || true
	@xcrun simctl launch 9C79A0B2-4025-4A4B-BECE-BB98F656118D "$(APP_ID)" 2>/dev/null || true
	@SIMULATOR_DEVICE_SET_PATH="$(SIM_SET)" open -a Simulator --args -CurrentDeviceUDID "$$(cat "$(SIM_STATE)")"
	@osascript -e 'tell application "Simulator" to activate' 2>/dev/null || true
	@xcrun devicectl device install app --device watch "$(APP)" 2>/dev/null && \
	  xcrun devicectl device process launch --device watch "$(APP_ID)" 2>/dev/null || true

tidy:
	@killall -9 Simulator 2>/dev/null || true
	@if [ -s "$(SIM_STATE)" ]; then \
	  xcrun simctl --set "$(SIM_SET)" shutdown "$$(cat "$(SIM_STATE)")" >/dev/null 2>&1 || true; \
	fi
	@$(MAKE) -C "$(DEPS)" tidy
	@$(MAKE) -C "$(LIBS)" tidy
	@$(MAKE) -C "$(OCIS)" tidy
	@rm -rf "$(DEVICE)/build" "$(DEVICE)/dist" "$(DIST)"

burn:
	@killall -9 Simulator 2>/dev/null || true
	@if [ -s "$(SIM_STATE)" ]; then \
	  xcrun simctl --set "$(SIM_SET)" shutdown "$$(cat "$(SIM_STATE)")" >/dev/null 2>&1 || true; \
	fi
	@$(MAKE) -C "$(DEPS)" burn
	@$(MAKE) -C "$(LIBS)" burn
	@$(MAKE) -C "$(OCIS)" burn
	@rm -rf "$(DEVICE)/build" "$(DEVICE)/dist" "$(DIST)"
