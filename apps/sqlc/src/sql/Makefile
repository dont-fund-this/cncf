LIB_NAME := sql
LIB_INCLUDES := -I../../../deps/sqlite/dist/include
LIB_LDFLAGS := -L../../../dist/lib -lcabi
LIB_LDLIBS := ../../../deps/sqlite/dist/lib/libsqlite3.a
SRC := $(shell find . -type f -name '*.cpp' \
        -not -path './build/*' \
        | sed 's|^./||' | sort -u)
include ../../common.mk
include ../../common.ios.mk

LIB_IOS_LDLIBS := ../../../deps/sqlite/dist-ios/lib/libsqlite3.a

.PHONY: ios-deps
ios-deps:
	@$(MAKE) -C ../../../deps/sqlite ios
ios: ios-deps
