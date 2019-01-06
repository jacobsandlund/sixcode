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

#    cmake .. -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD="host;JSBackend" -DLLVM_INCLUDE_EXAMPLES=OFF -DLLVM_INCLUDE_TESTS=OFF -DCLANG_INCLUDE_TESTS=OFF && \
#    make "-j$(nproc --all)" && \
#    cd /emscripten && \
#    git clone https://github.com/kripken/emscripten.git && \
#    git checkout $EMSCRIPTEN_VERSION && \
#    apt-get remove --purge \
#        build-essential \
#        cmake \
#        default-jre-headless \
#        && \
#    apt-get autoremove

RUN apt-get install -y \
        bash \
        clang \
        less \
        man-db \
        nginx \
        valgrind \
        ;

EXPOSE 8080

ENTRYPOINT ["development/entrypoint.sh"]
CMD ["./sixc", "start-server"]
