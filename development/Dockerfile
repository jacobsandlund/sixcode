FROM debian:9.6-slim

WORKDIR /spacetime

ENV LLVM_VERSION master
ENV EMSCRIPTEN_VERSION 1.38.40

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
    mkdir /deps && \
    cd /deps && git clone https://github.com/llvm/llvm-project && \
        cd llvm-project && git checkout $LLVM_VERSION && \
        mkdir build && cd build && \
        cmake \
            -DCMAKE_BUILD_TYPE=Release \
            -DLLVM_ENABLE_PROJECTS="clang;lld" \
            -DCMAKE_INSTALL_PREFIX=/usr/local \
            -DLLVM_TARGETS_TO_BUILD="host;WebAssembly" \
            -DLLVM_INCLUDE_EXAMPLES=OFF \
            -DLLVM_INCLUDE_TESTS=OFF \
            -DCLANG_INCLUDE_TESTS=OFF \
            -G "Unix Makefiles" \
            ../llvm && \
        make "-j$(nproc --all)" && make install && \
    cd /deps && git clone https://github.com/WebAssembly/binaryen && \
        cd binaryen && \
        cmake . && make "-j$(nproc --all)" && make install && \
    cd /deps && git clone https://github.com/emscripten-core/emscripten && \
        cd emscripten && git checkout $EMSCRIPTEN_VERSION && \
    cd /deps && \
        rm -rf binaryen && \
        rm -rf llvm-project && \
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

EXPOSE 8080

ENTRYPOINT ["development/entrypoint.sh"]
CMD ["development/start.sh"]
