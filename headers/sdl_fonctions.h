#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "structs.h"

#define NBSPRITES 17
#define VITESSE 1
#define RES_H 1280
#define RES_V 720
#define FPS 60

/**
 * \file sdl_fonctions.h
 * \brief Header pour la librairie de fonctions et les constantes liées à l'usage de la SDL2
 * \author Marie-Nina MUNAR L2 Info Le Mans
 * \version 1.1
 * \date 09/03/2020
*/

SDL_Window * initialisation_SDL();
void quitter_SDL(SDL_Window * fenetre);
int evenements(SDL_Window * fenetre);
int fullscreen_switch(SDL_Window * fenetre, int state);
void initialiser_sprites_personnages(SDL_Surface ** tab, int taille);
void afficher_surface(SDL_Surface * zone, SDL_Surface * sprite, position_t position);
void nettoyage_zone(SDL_Surface * zone);
void supprimer_sprites(SDL_Surface ** tab, int nb_sprites);
