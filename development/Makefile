.PHONY: clean

CFLAGS = -std=gnu11 -m32 -lm -Werror -Wall -Wextra
EMSCRIPTEN_FLAGS = -g4 -s ALLOW_MEMORY_GROWTH=1 -s USE_WEBGL2=1 -s GL_DEBUG=1
SRC = $(shell bin/ls-src | grep -v '\.h')

electron/sixcode.js: $(SRC)
	$(CC) -o $@ electron/sixcode.c $(EMSCRIPTEN_FLAGS) $(CFLAGS)

clean:
	@rm -f electron/sixcode.js electron/sixcode.was*
