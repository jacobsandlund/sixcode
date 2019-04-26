.PHONY: clean

CFLAGS = -std=gnu11 -m32 -lm -Werror -Wall -Wextra
EMSCRIPTEN_FLAGS = -lGLESv2 -g4 -s ALLOW_MEMORY_GROWTH=1 -s GL_DEBUG=1
SRC = $(shell find . -name '*.c' | grep -v '\-test.c')

electron/core.js: $(SRC)
	$(CC) -o $@ electron/core.c $(EMSCRIPTEN_FLAGS) $(CFLAGS)

watch:
	while true; do \
		bin/make; \
		echo '---'; \
		inotifywait -qq -e close_write -e modify -e move -e create -e delete *.c electron/*.c .; \
	done

clean:
	@rm -f electron/core.*
