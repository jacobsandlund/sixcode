FROM debian:9.6-slim

WORKDIR /sixcode

ENV EMSCRIPTEN_VERSION 1.38.31

RUN apt-get update && apt-get install -y curl gnupg2 && \
    curl -sL https://deb.nodesource.com/setup_12.x | bash - && \
    mkdir -p /usr/share/man/man1 && \
    apt-get install -y \
        build-essential \
        cmake \
        default-jre-headless \
        git \
        nodejs \
        python2.7 \
        && \
    mkdir /emscripten && \
    cd /emscripten && git clone https://github.com/kripken/emscripten-fastcomp && \
        cd emscripten-fastcomp && git checkout $EMSCRIPTEN_VERSION && \
        git clone https://github.com/kripken/emscripten-fastcomp-clang tools/clang && \
        cd tools/clang && git checkout $EMSCRIPTEN_VERSION && cd ../.. && \
        mkdir build && cd build && \
        cmake .. -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD="host;JSBackend" -DLLVM_INCLUDE_EXAMPLES=OFF -DLLVM_INCLUDE_TESTS=OFF -DCLANG_INCLUDE_TESTS=OFF && \
        make "-j$(nproc --all)" && make install && \
    cd /emscripten && git clone https://github.com/WebAssembly/binaryen && \
        cd binaryen && git checkout $EMSCRIPTEN_VERSION && \
        cmake . && make "-j$(nproc --all)" && make install && \
    cd /emscripten && git clone https://github.com/kripken/emscripten.git && \
        cd emscripten && git checkout $EMSCRIPTEN_VERSION && \
    cd /emscripten && \
        rm -rf binaryen && \
        rm -rf emscripten-fastcomp && \
        rm -rf emscripten/.git && \
    apt-get remove --purge -y \
        build-essential \
        cmake \
        default-jre-headless \
        && \
    apt-get autoremove -y

RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get install -y \
        bash \
        binutils \
        inotify-tools \
        less \
        libc6-dbg:i386 \
        libc6-dev-i386 \
        libgcc-6-dev \
        make \
        man-db \
        valgrind \
        ;

# TODO: remove
RUN apt-get update && \
    apt-get install -y \
        libgles2-mesa-dev:i386 \
        ;

COPY development/emscripten_config /root/.emscripten

RUN mkdir electron/

COPY *.c *.h development/Makefile ./
COPY electron/*.c electron/*.h ./electron/

RUN /emscripten/emscripten/emmake make && \
    rm *.c *.h Makefile && rm -r electron

EXPOSE 8080

ENTRYPOINT ["development/entrypoint.sh"]
CMD ["development/start.sh"]
