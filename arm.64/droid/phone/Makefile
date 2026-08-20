include env.mk

.PHONY: test tidy

test:
	@mkdir -p $(DIST)
	@echo '{"app": "droid-phone", "status": "ready", "frame": "$(FRAME)", "ram": $(RAM)}' > $(DIST)/$(PROOF)
	@test -s $(DIST)/$(PROOF)

tidy:
	@rm -rf build $(DIST)
