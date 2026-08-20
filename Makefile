include env.mk

.PHONY: test tidy

test:
# 	$(MAKE) -C deps test
	$(MAKE) -C apps test
# 	$(MAKE) -C lint test
	$(MAKE) -C arm.64 test

tidy:
# 	$(MAKE) -C deps tidy
	$(MAKE) -C apps tidy
# 	$(MAKE) -C lint tidy
	$(MAKE) -C arm.64 tidy
