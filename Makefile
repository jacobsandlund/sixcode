SRC = $(shell find . -name '*.c' | grep -v '\-test.c')
CFLAGS = -std=gnu11 -lm -Werror -Wall -Wextra

web/core.js: $(SRC)
	$(CC) -o $@ $^ $(CFLAGS)
