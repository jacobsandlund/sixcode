.PHONY: clean

CFLAGS = -O2 -std=gnu11 -m32 -lm -Werror -Wall -Wextra
EMSCRIPTEN_FLAGS = -g4 -s ALLOW_MEMORY_GROWTH=1 -s USE_WEBGL2=1
SRC = $(shell bin/ls-src | grep -v '\.h')

electron/spacetime.js: $(SRC)
	$(CC) -o $@ electron/spacetime.c $(EMSCRIPTEN_FLAGS) $(CFLAGS)

clean:
	@rm -f electron/spacetime.js electron/spacetime.was*
