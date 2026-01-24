#!/bin/sh

# This is my specific compilation process. 
# Read the wiki for more info: https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)

set -xe
mkdir -p output

if [ "$1" = "web" ]; then
    # Note: I don't use optimization flags because I want the generated html and js code to be hackable (also from my website!). 
    # Therefore I don't want the .html and .js files to be minified.
    # Once the files are generated, I move them to my website.
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

    # Other Option: preloading of the resources
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

