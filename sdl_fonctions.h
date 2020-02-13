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
SDL_Surface ** initialiser_sprites_personnages(SDL_Surface ** tab, int taille, char* nom);
void afficher_surface(SDL_Surface * zone, SDL_Surface * sprite, position_t position);
void nettoyage_zone(SDL_Surface * zone);
void supprimer_sprites(SDL_Surface ** tab, int nb_sprites);
