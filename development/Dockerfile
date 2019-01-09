FROM debian:9.6-slim

WORKDIR /sixcode

ENV EMSCRIPTEN_VERSION 1.38.21

RUN apt-get update && apt-get install -y curl gnupg2 && \
    curl -sL https://deb.nodesource.com/setup_10.x | bash - && \
    mkdir -p /usr/share/man/man1 && \
    apt-get install -y \
        build-essential \
        cmake \
        default-jre-headless \
        git \
        nodejs \
        python2.7 \
        && \
    mkdir /emscripten


RUN cd /emscripten && \
    git clone https://github.com/kripken/emscripten-fastcomp && \
    cd emscripten-fastcomp && git checkout $EMSCRIPTEN_VERSION && \
    git clone https://github.com/kripken/emscripten-fastcomp-clang tools/clang && \
    cd tools/clang && git checkout $EMSCRIPTEN_VERSION && cd ../.. && \
    mkdir build && cd build

RUN cd /emscripten/emscripten-fastcomp/build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD="host;JSBackend" -DLLVM_INCLUDE_EXAMPLES=OFF -DLLVM_INCLUDE_TESTS=OFF -DCLANG_INCLUDE_TESTS=OFF && \
    make "-j$(nproc --all)"

RUN cd /emscripten && \
    git clone https://github.com/kripken/emscripten.git && \
    cd emscripten && git checkout $EMSCRIPTEN_VERSION

RUN cd /emscripten && \
    git clone https://github.com/WebAssembly/binaryen && \
    cd binaryen && git checkout $EMSCRIPTEN_VERSION && \
    cmake . && make "-j$(nproc --all)"


RUN cd /emscripten/emscripten-fastcomp/build && make install && \
    cd /emscripten/binaryen && make install && \
    cd /emscripten && rm -rf binaryen && rm -rf emscripten-fastcomp

RUN apt-get remove --purge -y \
        build-essential \
        cmake \
        default-jre-headless \
        && \
    apt-get autoremove -y

RUN apt-get install -y \
        bash \
        binutils \
        less \
        libc6-dev \
        libgcc-6-dev \
        man-db \
        nginx \
        valgrind \
        ;

COPY development/emscripten_config /root/.emscripten

COPY *.c *.h ./

RUN /emscripten/emscripten/emcc interface-js.c -o core.js && \
    rm *

EXPOSE 8080

ENTRYPOINT ["development/entrypoint.sh"]
CMD ["development/bin/start-server"]
