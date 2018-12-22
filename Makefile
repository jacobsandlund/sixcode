.PHONY: test

test:
	@find . -name '*-test.c' | xargs -n 1 ./run && git diff --exit-code -- *-test.c */*-test.c
