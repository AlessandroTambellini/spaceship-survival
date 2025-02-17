# spaceship-survival
2D arcade game built in C with Raylib

## Compilation
The code is structured to be compilable for both the desktop and the web enviornment (WebAssembly)  
Link to the wiki page of Raylib: https://github.com/raysan5/raylib/wiki

## Notes
### Styling
I messed around a bit with styling: trying to organize it in a CSS-like structs, defining style macros, custom components, etc. I was not sure how to organize it.  
I may have felt into the trap of refactoring :( 

### Collision
The collision between the spaceship and the asteroid is approximated with the collision of a rectangle and a circle. 

### Propulsion Sound
Initially, I set that if 'w' key stays pressed for more than the duration of the propulsion sound,
the latter was going to restart and loop. But, the restart of the sound was clearly hearable and ugly and so
I decided not to loop the sound once it finishes.  
P.S. I know in space there is no sound. Still, I wanted a propulsion sound. 

### Attributes
* [Song icons created by abdul allib - Flaticon](https://www.flaticon.com/free-icons/song)
* [Silence icons created by abdul allib - Flaticon](https://www.flaticon.com/free-icons/silence)
* [Sound Effect from Pixabay]("https://pixabay.com/?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=47562")
