#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "constantes_SDL.h"
#include "structs.h"


SDL_Window * initialisation_SDL();
void quitter_SDL(SDL_Window * fenetre);
int evenements(SDL_Window * fenetre);
int fullscreen_switch(SDL_Window * fenetre, int state);
