# Spaceship Survival
Spaceship Survival, 2D arcade game.

## Compilation
### GNU Linux
`cc -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 game.c -o game`
### Web (HTML5)
```sh
emcc -Wall ~/raylib/src/libraylib.a -I. -I/usr/include -L. -L~/raylib/src/libraylib.a -s USE_GLFW=3 --shell-file shell.html -DPLATFORM_WEB --preload-file resources -sASSERTIONS -sINITIAL_MEMORY=25MB -sSTACK_SIZE=5MB -o game.html game.c
```
This is my specific compilation process. Read the Web wiki for more info: https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)

## Notes
### Collision
The collision between the spaceship and the asteroid is approximated with the collision of a rectangle and a circle. 

### Propulsion Sound
Initially, I set that if 'w' key stays pressed for more than the duration of the propulsion sound,
the latter was going to restart and loop. But, the restart of the sound was clearly hearable and ugly and so
I decided not to loop the sound once it finishes.  
P.S. I know in space there is no sound. Still, I wanted a propulsion sound :) 

### Attributes
* [Sound Effect from Pixabay]("https://pixabay.com/?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=47562")
