include env.mk

.PHONY: test tidy

test:
	@$(MAKE) -C "$(DEPS)" test
	@$(MAKE) -C "$(LIBS)" test
	@$(MAKE) -C "$(OCIS)" test
	@mkdir -p "$(APP)/Frameworks/libbox.framework"
	@mkdir -p "$(APP)/Frameworks/libefs.framework"
	@mkdir -p "$(APP)/Frameworks/libjam.framework"
	@mkdir -p "$(APP)/Frameworks/libtui.framework"
	@cp "$(LIB_DIST)/libbox" "$(APP)/Frameworks/libbox.framework/libbox"
	@cp "$(LIB_DIST)/libefs" "$(APP)/Frameworks/libefs.framework/libefs"
	@cp "$(LIB_DIST)/libjam" "$(APP)/Frameworks/libjam.framework/libjam"
	@cp "$(LIB_DIST)/libtui" "$(APP)/Frameworks/libtui.framework/libtui"
	@chmod +x "$(APP)/Frameworks/libbox.framework/libbox"
	@chmod +x "$(APP)/Frameworks/libefs.framework/libefs"
	@chmod +x "$(APP)/Frameworks/libjam.framework/libjam"
	@chmod +x "$(APP)/Frameworks/libtui.framework/libtui"
	@printf '<?xml version="1.0" encoding="UTF-8"?>\n<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">\n<plist version="1.0">\n<dict>\n    <key>CFBundleIdentifier</key>\n    <string>dev.pat.libbox</string>\n    <key>CFBundleExecutable</key>\n    <string>libbox</string>\n    <key>CFBundlePackageType</key>\n    <string>FMWK</string>\n    <key>CFBundleShortVersionString</key>\n    <string>1.0</string>\n    <key>CFBundleVersion</key>\n    <string>1</string>\n    <key>MinimumOSVersion</key>\n    <string>17.0</string>\n</dict>\n</plist>\n' > "$(APP)/Frameworks/libbox.framework/Info.plist"
	@printf '<?xml version="1.0" encoding="UTF-8"?>\n<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">\n<plist version="1.0">\n<dict>\n    <key>CFBundleIdentifier</key>\n    <string>dev.pat.libefs</string>\n    <key>CFBundleExecutable</key>\n    <string>libefs</string>\n    <key>CFBundlePackageType</key>\n    <string>FMWK</string>\n    <key>CFBundleShortVersionString</key>\n    <string>1.0</string>\n    <key>CFBundleVersion</key>\n    <string>1</string>\n    <key>MinimumOSVersion</key>\n    <string>17.0</string>\n</dict>\n</plist>\n' > "$(APP)/Frameworks/libefs.framework/Info.plist"
	@printf '<?xml version="1.0" encoding="UTF-8"?>\n<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">\n<plist version="1.0">\n<dict>\n    <key>CFBundleIdentifier</key>\n    <string>dev.pat.libjam</string>\n    <key>CFBundleExecutable</key>\n    <string>libjam</string>\n    <key>CFBundlePackageType</key>\n    <string>FMWK</string>\n    <key>CFBundleShortVersionString</key>\n    <string>1.0</string>\n    <key>CFBundleVersion</key>\n    <string>1</string>\n    <key>MinimumOSVersion</key>\n    <string>17.0</string>\n</dict>\n</plist>\n' > "$(APP)/Frameworks/libjam.framework/Info.plist"
	@printf '<?xml version="1.0" encoding="UTF-8"?>\n<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">\n<plist version="1.0">\n<dict>\n    <key>CFBundleIdentifier</key>\n    <string>dev.pat.libtui</string>\n    <key>CFBundleExecutable</key>\n    <string>libtui</string>\n    <key>CFBundlePackageType</key>\n    <string>FMWK</string>\n    <key>CFBundleShortVersionString</key>\n    <string>1.0</string>\n    <key>CFBundleVersion</key>\n    <string>1</string>\n    <key>MinimumOSVersion</key>\n    <string>17.0</string>\n</dict>\n</plist>\n' > "$(APP)/Frameworks/libtui.framework/Info.plist"
	@codesign --force --sign - "$(APP)/Frameworks/libbox.framework"
	@codesign --force --sign - "$(APP)/Frameworks/libefs.framework"
	@codesign --force --sign - "$(APP)/Frameworks/libjam.framework"
	@codesign --force --sign - "$(APP)/Frameworks/libtui.framework"
	@xcrun -sdk "$(SDK)" swiftc -parse-as-library -target "$(TRIPLE)" \
	  -import-objc-header src/bridge.h \
	  -Xlinker -rpath -Xlinker @executable_path/Frameworks \
	  $$(find src -name '*.swift') -o "$(APP)/pat"
	@cp "$(INFO)" "$(APP)/Info.plist"
	@cp "$(OCIS)/guest/build/out/bios64.bin" "$(APP)/bios64.bin"
	@cp "$(OCIS)/guest/build/out/risc64.bin" "$(APP)/risc64.bin"
	@cp "$(OCIS)/guest/build/out/rootfs.ext2" "$(APP)/rootfs.ext2"
	@codesign --force --sign - "$(APP)"
	@xcrun simctl install "$(SIM)" "$(APP)"
	@xcrun simctl launch "$(SIM)" "$(APP_ID)"

tidy:
	@$(MAKE) -C "$(DEPS)" tidy
	@$(MAKE) -C "$(LIBS)" tidy
	@$(MAKE) -C "$(OCIS)" tidy
	@xcrun simctl shutdown "$(SIM)" 2>/dev/null || true
	@rm -rf "$(DEVICE)/build" "$(DIST)"
