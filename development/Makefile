.PHONY: clean

CFLAGS = -std=gnu11 -m32 -lm -O1 -Werror -Wall -Wextra
EMSCRIPTEN_FLAGS = -g4 -s ALLOW_MEMORY_GROWTH=1 -s USE_WEBGL2=1 -s
SRC = $(shell bin/ls-src | grep -v '\.h')

spacetime.js: $(SRC)
	$(CC) -o $@ spacetime.c $(EMSCRIPTEN_FLAGS) $(CFLAGS)

clean:
	@rm -f spacetime.js spacetime.was*
