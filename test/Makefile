SHELL := /bin/bash
include env.mk

.PHONY: test tidy

test:
	@lsof -ti :6443 | xargs kill -9 2>/dev/null || true

	@[ -f ./kubernetes/_output/bin/e2e.test ] || (cd kubernetes && go test -c ./test/e2e -o _output/bin/e2e.test)
	@[ -f ./hydrophone/hydrophone ] || (cd hydrophone && go build -o hydrophone .)
	
	LISTEN_TEST=127.0.0.1:6443 npm --prefix ../arm.64/langs/node run pump &

	./kubernetes/_output/bin/e2e.test \
		--host=http://127.0.0.1:6443 \
		--provider=skeleton \
		--ginkgo.focus="should return version info"

	./hydrophone/hydrophone \
		--conformance \
		--kubeconfig=<(echo '{"apiVersion":"v1","clusters":[{"cluster":{"server":"http://127.0.0.1:6443"},"name":"fog"}],"contexts":[{"context":{"cluster":"fog","user":"default"},"name":"fog"}],"current-context":"fog"}') \
		--output-dir=./conformance/v1.36/fog

tidy:
	@lsof -ti :6443 | xargs kill -9 2>/dev/null || true
