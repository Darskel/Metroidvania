#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL2.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "constantes_SDL.h"

int initialisation_SDL(){
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    fprintf(stdout, "Echec de l'initalisation de la SDL (%s)\n", SDL_GetError());
    return -1;
  }
  
}
