FROM alpine:3.8

WORKDIR /sixcode

RUN apk --no-cache add \
        bash \
        build-base \
        clang \
        git \
        git-daemon \
        less \
        man \
        nodejs \
        nginx \
        valgrind \
        ;

RUN mkdir -p /run/nginx

EXPOSE 8080

ENTRYPOINT ["development/entrypoint.sh"]
CMD ["bin/start-server"]
