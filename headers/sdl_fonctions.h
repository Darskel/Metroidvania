#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "../headers/structs.h"
#include "../headers/source.h"
#include "../headers/comportement.h"

/**
 * \file sdl_fonctions.h
 * \brief Header pour la librairie de fonctions et les constantes liées à l'usage de la SDL2
 * \author Marie-Nina MUNAR L2 Info Le Mans
 * \version 4.0
 * \date 06/04/2020
*/

void initialisation_SDL(SDL_Window ** fenetre, SDL_Renderer ** renderer, SDL_DisplayMode * mode, boolean_t fullscreen);
void quitter_SDL(SDL_Window ** fenetre, SDL_Renderer ** renderer);
void evenements(int fullscreen);
SDL_Texture * initialiser_texture(char * path, SDL_Renderer * renderer);
personnage_t * initialisation_personnage(SDL_Renderer * renderer, position_t positionDepart, position_t positionDepartDelta);
salle_t * initialiser_salle(SDL_Renderer * renderer, char* nomFichier, SDL_Texture * tileset);
void afficher_salle(SDL_Renderer * renderer, salle_t * salle);
void afficher_personnage(SDL_Renderer * renderer, personnage_t * personnage, salle_t * salle);
void affichage_complet(SDL_Renderer * renderer, salle_t * salle, personnage_t * personnage);
void miseAjourSprites(personnage_t * personnage);
void destroy_salle(salle_t ** salle);
void destroy_personnage(personnage_t ** perso);
void ecranNoir(SDL_Renderer * renderer, int ms);
void konamicode(personnage_t * perso, salle_t * salle, char * konami, int * indKon, boolean_t * kon);
