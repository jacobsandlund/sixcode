FROM debian:9.6-slim

WORKDIR /spacetime
ENV SPACETIME_PATH /spacetime

RUN apt-get update && \
    apt-get install -y \
        bash \
        clang \
        valgrind \
        ;
