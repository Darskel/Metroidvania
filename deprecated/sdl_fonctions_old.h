#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "structs.h"

#define RES_H 1280
#define RES_V 720
#define TEMPSSPRITE 2
#define TEMPSSAUT 25
#define FPS 50
#define FRAMEDELAY 1000/FPS
#define LIMITS 30
#define NOM_JEU "Diskosieni"
#define TAILLEBLOC 8

/** Pas inclut dans la doc*/

SDL_Window * initialisation_SDL();
void quitter_SDL(SDL_Window * fenetre);
int evenements(SDL_Window * fenetre);
void initialiser_salle(salle_t * s);
void initialiser_sprites_personnages(personnage_t * p);
void initialiser_sprites_salle(salle_t * s);
personnage_t * initialisation_personnage();
void afficher_surface(SDL_Surface * zone, SDL_Surface * sprite, position_t position);
void afficher_salle(SDL_Surface * zone, salle_t * salle);
void nettoyage_zone(SDL_Surface * zone);
void supprimer_sprites(SDL_Surface ** tab, int nb_sprites);
