#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef SYSTEM_INCLUDED
#define SYSTEM_INCLUDED

#if defined(_WIN32)
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>
    #include <SDL2/SDL_image.h>
    //#include <SDL2/SDL_mixer.h>
#else
    #include <SDL.h>
    #include <SDL_ttf.h>
    #include <SDL_image.h>
    //#include <SDL_mixer.h>
#endif

#endif
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
SDL_Texture * initialiser_texture(char * path, SDL_Renderer * renderer);
personnage_t * initialisation_personnage(SDL_Renderer * renderer, position_t positionDepart, position_t positionDepartDelta);
salle_t * initialiser_salle(SDL_Renderer * renderer, char* nomFichier);
void afficher_salle(SDL_Renderer * renderer, salle_t * salle, SDL_Texture * textureSalle);
void afficher_personnage(SDL_Renderer * renderer, personnage_t * personnage, salle_t * salle, SDL_Texture * textureSalle);
void afficher_entites(SDL_Renderer * renderer, salle_t * salle, SDL_Texture * textureSalle);
void affichage_complet(SDL_Renderer * renderer, salle_t * salle, personnage_t * personnage);
void miseAjourSprites(personnage_t * personnage);
void miseAjourSpritesEntites(salle_t * salle);
void destroy_salle(salle_t ** salle);
void destroy_personnage(personnage_t ** perso);
void ecranNoir(SDL_Renderer * renderer, int ms);
void konamicode(personnage_t * perso, salle_t * salle, char * konami, int * indKon, boolean_t * kon);
void destroy_typeentites(void);
void initialiser_typeentites(SDL_Renderer * renderer);
void chargerSauvegardeMenu(void);
void nouvellePartieMenu(void);
void sauvegarderMenu(void);
void jeu(SDL_Window * fenetre, SDL_Renderer ** renderer, SDL_DisplayMode mode, SDL_Joystick * pJoystick, int fullscreen);
void afficher_menu(SDL_Renderer * renderer, SDL_Texture * fond);
int afficherMessageBox(SDL_Window * fenetre, SDL_MessageBoxButtonData * buttons, int nbButtons, char * titre, char * message, int fullscreen);
menu_t * creerMenuDemarrage(SDL_Renderer * renderer);
menu_t * creerMenuInGame(SDL_Renderer * renderer);
menu_t * creerMenuConfirmation(SDL_Renderer * renderer);
void detruireMenu(menu_t ** menu);
boolean_t menuConfirmation(SDL_Renderer * renderer);
void afficherMenu(SDL_Renderer * renderer, menu_t * menu);
void afficherVieJauge(SDL_Renderer * renderer, personnage_t * personnage, SDL_Texture * textureSalle);
void afficherVieCoeurs(SDL_Renderer * renderer, personnage_t * personnage, SDL_Texture * coeurImage, SDL_Texture * textureSalle);
