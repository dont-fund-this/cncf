SHELL := /bin/bash
include env.mk

.PHONY: test tidy

test:
	@[ -d hydrophone ] 			|| ([ -d ~/_pm_/hydrophone ] 		 && cp -r ~/_pm_/hydrophone . 			|| git clone --depth 1 --branch $(HYDROPHONE_REV) 		  https://github.com/kubernetes-sigs/hydrophone.git)
	@[ -d k8s-conformance ] 	|| ([ -d ~/_pm_/k8s-conformance ] 	 && cp -r ~/_pm_/k8s-conformance . 		|| git clone --depth 1 --branch $(K8S_CONFORMANCE_REV) 	  https://github.com/cncf/k8s-conformance.git)
	@[ -d kubernetes ] 			|| ([ -d ~/_pm_/kubernetes ] 		 && cp -r ~/_pm_/kubernetes . 			|| git clone --depth 1 --branch $(KUBERNETES_REV) 		  https://github.com/kubernetes/kubernetes.git)
	@[ -d serde-json-core ] 	|| ([ -d ~/_pm_/serde-json-core ] 	 && cp -r ~/_pm_/serde-json-core . 		|| git clone --depth 1 --branch $(SERDE_JSON_CORE_REV)    https://github.com/rust-embedded-community/serde-json-core.git)
	@[ -d tree-sitter ] 		|| ([ -d ~/_pm_/tree-sitter ] 		 && cp -r ~/_pm_/tree-sitter . 			|| git clone --depth 1 --branch $(TREE_SITTER_REV) 	      https://github.com/tree-sitter/tree-sitter.git)
	@[ -d tree-sitter-cpp ] 	|| ([ -d ~/_pm_/tree-sitter-cpp ] 	 && cp -r ~/_pm_/tree-sitter-cpp . 		|| git clone --depth 1 --branch $(TREE_SITTER_CPP_REV) 	  https://github.com/tree-sitter/tree-sitter-cpp.git)
	@[ -d tree-sitter-go ] 		|| ([ -d ~/_pm_/tree-sitter-go ] 	 && cp -r ~/_pm_/tree-sitter-go . 		|| git clone --depth 1 --branch $(TREE_SITTER_GO_REV) 	  https://github.com/tree-sitter/tree-sitter-go.git)
	@[ -d tree-sitter-rust ] 	|| ([ -d ~/_pm_/tree-sitter-rust ] 	 && cp -r ~/_pm_/tree-sitter-rust . 	|| git clone --depth 1 --branch $(TREE_SITTER_RUST_REV)   https://github.com/tree-sitter/tree-sitter-rust.git)
	@[ -d verify-conformance ] 	|| ([ -d ~/_pm_/verify-conformance ] && cp -r ~/_pm_/verify-conformance . 	|| git clone --depth 1 --branch $(VERIFY_CONFORMANCE_REV) https://github.com/cncf-infra/verify-conformance.git)

	@[ -f ./kubernetes/_output/bin/e2e.test ] || (cd kubernetes && go test -c ./test/e2e -o _output/bin/e2e.test)

	@[ -f ./hydrophone/hydrophone ] || (cd hydrophone && go build -o hydrophone .)

tidy:
