.PHONY: clean

CFLAGS = -std=gnu11 -lm -Werror -Wall -Wextra
SRC = $(shell find . -name '*.c' | grep -v '\-test.c')

web/core.js: $(SRC)
	$(CC) -o $@ $^ $(CFLAGS)



clean:
	@rm -f web/core.js
	@rm -f web/core.wasm
