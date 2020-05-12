#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SDL_H
#define SDL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

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

#define NBSOUNDS 9
Mix_Chunk * EffetsSonores[NBSOUNDS];
float pourcentageVol;

void initialiserChunks(void);
void detruireChunks(void);
void initialisation_SDL(SDL_Window ** fenetre, SDL_Renderer ** renderer, SDL_DisplayMode * mode, boolean_t fullscreen);
void quitter_SDL(SDL_Window ** fenetre, SDL_Renderer ** renderer);
SDL_Texture * initialiser_texture(char * path, SDL_Renderer * renderer, boolean_t estTarget);
personnage_t * initialisation_personnage(SDL_Renderer * renderer, position_t positionDepart, position_t positionDepartDelta);
salle_t * initialiser_salle(SDL_Renderer * renderer, char* nomFichier, personnage_t* perso);
void afficher_salle(SDL_Renderer * renderer, salle_t * salle, SDL_Texture * textureSalle);
void afficher_personnage(SDL_Renderer * renderer, personnage_t * personnage, salle_t * salle, SDL_Texture * textureSalle);
void afficher_entites(SDL_Renderer * renderer, salle_t * salle, SDL_Texture * textureSalle);
void affichage_complet(SDL_Renderer * renderer, salle_t * salle, personnage_t * personnage, int * inventaireAffiche);
void miseAjourSprites(personnage_t * personnage);
void miseAjourSpritesEntites(salle_t * salle);
void evoSalle(salle_t * salle);
void destroy_salle(salle_t ** salle);
void destroy_personnage(personnage_t ** perso);
void ecranNoir(SDL_Renderer * renderer, int ms);
void konamicode(personnage_t * perso, salle_t * salle, char * konami, int * indKon, boolean_t * kon);
void destroy_typeentites(void);
void initialiser_typeentites(SDL_Renderer * renderer);
int chargerSauvegardeMenu(SDL_Renderer * renderer, int numSauv, personnage_t ** perso, salle_t ** salle);
int sauvegarderMenu(int numSauv, personnage_t * perso, salle_t * salle);
boolean_t jeu(SDL_Window * fenetre, SDL_Renderer ** renderer, SDL_DisplayMode mode, SDL_Joystick * pJoystick, int fullscreen);
menu_t * creerMenuDemarrage(SDL_Renderer * renderer);
menu_t * creerMenuInGame(SDL_Renderer * renderer);
menu_t * creerMenuConfirmation(SDL_Renderer * renderer, char * question);
void detruireMenu(menu_t ** menu);
boolean_t menuConfirmation(SDL_Renderer * renderer, char * question, int tailleTexte, int tailleBoutons);
void afficher_menu(SDL_Renderer * renderer, menu_t * menu, int tailleTexte, int tailleBoutons, boolean_t sens);
void TouchesMenu(int direction, SDL_Point souris, boolean_t bougeSouris, menu_t * menu);
int evoMenu(menu_t * menu, int clique);
void afficherVieJauge(SDL_Renderer * renderer, personnage_t * personnage, SDL_Texture * textureSalle);
void afficherVieCoeurs(SDL_Renderer * renderer, personnage_t * personnage, SDL_Texture * coeurImage, SDL_Texture * textureSalle);
void afficherInventaire(SDL_Renderer * renderer, personnage_t * personnage, SDL_Texture * textureSalle);
void afficherVolume(SDL_Renderer * renderer);
void gameover(SDL_Window * fenetre, SDL_Renderer * renderer, SDL_DisplayMode mode, SDL_Joystick * pJoystick, int fullscreen);
TTF_Font * creerPolice(char * path, int taille);
void detruirePolice(TTF_Font ** font);
SDL_Texture * creerTexte(SDL_Renderer * renderer, char * Fontpath, int taille, char* texte, int r, int g, int b);
Mix_Music * chargerMusique(char * path);
void lancerMusiqueInfini(Mix_Music * musique);
void lancerMusiqueNBFois(Mix_Music * musique, int nbFois);
void togglePauseMusique(void);
void MonterSon(void);
void baisserSon(void);
void ExploitationBinaireSons(unsigned int nbBinaire);
void detruireTexturesMenu(menu_t * menu);
void creerTexturesMenuDemarrage(SDL_Renderer * renderer, menu_t * menu);
void creerTexturesMenuConfirmation(SDL_Renderer * renderer, menu_t * menu);

//Ancienne version de l'audio sans SDL_Mixer
/*void Audiocallback(void *userdata, Uint8 *stream, int len);
audiodata_t * chargerWAVreplay(char * fichier);
void finMusique(audiodata_t ** audiodata);
void lectureMusique(audiodata_t * audiodata);
void pauseMusique(audiodata_t * audiodata);
void togglePauseMusic(audiodata_t * audiodata);*/
