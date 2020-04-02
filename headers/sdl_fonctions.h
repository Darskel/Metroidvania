#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../headers/structs.h"
#include "../headers/source.h"
#include "../headers/comportement.h"

#define NOM_JEU "Diskosieni"
#define FRAMEDELAY 20
#define PLAYERSPRITESPATH "./sprites/entite/joueur/sprite_joueur.png"
#define TILESETPATH "./sprites/bloc/tileset.png"
#define TAILLEBLOC 8
#define HAUTEURPORTE 36
#define LARGEURPORTE 13
#define HAUTEURSPRITEPERS 33
#define LARGEURSPRITEPERS 30
#define HAUTEURHITBOXPERS 4
#define LARGEURHITBOXPERS 2
#define NIVEAUTXT "salle_debut.txt"
#define NIVEAUBG "./sprites/salles/salle_debut.png"
#define DUREESPRITE 2


/**
 * \file sdl_fonctions.h
 * \brief Header pour la librairie de fonctions et les constantes liées à l'usage de la SDL2
 * \author Marie-Nina MUNAR L2 Info Le Mans
 * \version 3.0
 * \date 26/03/2020
*/

void initialisation_SDL(SDL_Window ** fenetre, SDL_Renderer ** renderer, SDL_DisplayMode * mode, boolean_t fullscreen);
void quitter_SDL(SDL_Window ** fenetre, SDL_Renderer ** renderer);
void evenements(SDL_Renderer * renderer, SDL_DisplayMode * mode);
SDL_Texture * initialiser_texture(char * path, SDL_Renderer * renderer);
personnage_t * initialisation_personnage(SDL_Renderer * renderer, position_t positionDepart, position_t positionDepartDelta);
salle_t * initialiser_salle(SDL_Renderer * renderer, char* nomFichier, char * nomBG, SDL_Texture * tileset);
void afficher_salle(SDL_Renderer * renderer, salle_t * salle);
void afficher_personnage(SDL_Renderer * renderer, personnage_t * personnage, salle_t * salle);
void affichage_complet(SDL_Renderer * renderer, salle_t * salle, personnage_t * personnage);
void miseAjourSprites(personnage_t * personnage);
void destroy_salle(salle_t ** salle);
void destroy_personnage(personnage_t ** perso);
