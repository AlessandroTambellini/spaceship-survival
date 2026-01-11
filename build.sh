#!/bin/sh

# This is my specific compilation process. 
# Read the wiki for more info: https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)

set -xe
mkdir -p output

if [ "$1" = "web" ]; then
    # -O2
    # -sASSERTIONS
    emcc \
        -Wall \
        -I/usr/include \
        -sUSE_GLFW=3 \
        -sINITIAL_MEMORY=32MB \
        -sSTACK_SIZE=5MB \
        -DPLATFORM_WEB \
        --shell-file shell.html \
        --embed-file resources \
        -o output/spaceship-survival.html \
        ~/raylib/src/libraylib.a spaceship-survival.c

    # Option: preloading of the resources
    # emcc \
    #     -Wall \
    #     -I/usr/include \
    #     -DPLATFORM_WEB  \
    #     -sUSE_GLFW=3  \
    #     -sINITIAL_MEMORY=25MB  \
    #     -sSTACK_SIZE=5MB  \
    #     --shell-file shell.html  \
    #     --preload-file resources  \
    #     -o output/spaceship-survival.html  \
    #     ~/raylib/src/libraylib.a spaceship-survival.c 
else
    cc -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 spaceship-survival.c -o output/spaceship-survival
fi

