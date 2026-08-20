ROOT_DIR            ?= /Users/bubbles/dev/cncf
DIST_DIR            ?= $(ROOT_DIR)/dist
DEPS_DIR            ?= $(ROOT_DIR)/deps
CONFORMANCE_VERSION ?= v1.36
E2E_TEST            ?= $(ROOT_DIR)/deps/kubernetes/_output/bin/e2e.test
HYDROPHONE          ?= $(ROOT_DIR)/deps/hydrophone/hydrophone
E2E_FOCUS           ?= should return version info