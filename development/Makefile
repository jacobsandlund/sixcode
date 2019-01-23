.PHONY: clean

CFLAGS = -std=gnu11 -m32 -lm -Werror -Wall -Wextra
EMSCRIPTEN_FLAGS = -lGLESv2 -g4 -s ALLOW_MEMORY_GROWTH=1 -s GL_DEBUG=1
SRC = $(shell find . -name '*.c' | grep -v '\-test.c')

web/core.js: $(SRC)
	$(CC) -o $@ web.c $(EMSCRIPTEN_FLAGS) $(CFLAGS)

clean:
	@rm -f web/core.*
