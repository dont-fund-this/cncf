include env.mk

.PHONY: test tidy

test:
	$(MAKE) -C electron test
	$(MAKE) -C cef      test
	$(MAKE) -C qt       test
	$(MAKE) -C blazor   test
	$(MAKE) -C avalonia test
	$(MAKE) -C metal    test
	$(MAKE) -C slint    test

tidy:
	$(MAKE) -C electron tidy 
	$(MAKE) -C cef      tidy 
	$(MAKE) -C qt       tidy 
	$(MAKE) -C blazor   tidy 
	$(MAKE) -C avalonia tidy 
	$(MAKE) -C metal    tidy 
	$(MAKE) -C slint    tidy 
