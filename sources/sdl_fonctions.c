#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../headers/sdl_fonctions.h"
#include "../headers/structs.h"

/**
 * \file sdl_fonctions.c
 * \brief Fichier qui regroupent les fonctions utilisées pour la gestion des graphismes (SDL2)
 * \author Marie-Nina MUNAR L2 Info Le Mans
 * \version 1.3
 * \date 09/03/2020
*/

/**
 * \brief Fonction d'initalisation de la SDL et de création de la fenetre
 *
 * @return le pointeur sur la fenetre SDL_Window créée
 */
SDL_Window * initialisation_SDL(){
  SDL_Window * fenetre = NULL;

  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    fprintf(stderr, "Echec de l'initalisation de la SDL (%s)\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  if(TTF_Init () ==  -1){
    fprintf(stderr, "Erreur d’initialisation de TTF_Init : %s\n", TTF_GetError ());
    exit(EXIT_FAILURE);
  }

  fenetre = SDL_CreateWindow("Metroidvania", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RES_H, RES_V, SDL_WINDOW_SHOWN);
  if(!fenetre){
    fprintf(stderr, "Erreur de creation de la fenetre : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  return fenetre;
}

/**
 * \brief Fonction qui permet de détruire la fenetre et quitter la SDL
 *
 * @param fenetre le pointeur sur la fenetre SDL_Window utilisée
 */
void quitter_SDL(SDL_Window * fenetre){
  SDL_DestroyWindow(fenetre);
  SDL_Quit();
}

/**
 * \brief Fonction qui permet de basculer du plein écran au fenetré
 *
 * @param fenetre le pointeur sur la fenetre SDL_Window utilisée
 * @param state l'état actuel de la fenetre (plein ecran ou fenetré)
 * @return l'entier indiquant le nouvel état de la fenetre
 */
int fullscreen_switch(SDL_Window * fenetre, int state){
  if (state){
    SDL_SetWindowFullscreen(fenetre,0);
    return 0;
    }
  else{
    SDL_SetWindowFullscreen(fenetre,SDL_WINDOW_FULLSCREEN);
    return 1;
    }
}

/**
 * \brief Fonction qui permet d'initialiser un tableau de sprites
 *
 * @param tab tableau de sprites à intialiser
 * @param taille la taille du tableau à initialiser
 */
void initialiser_sprites_personnages(SDL_Surface ** tab, int taille){

  tab[SU]=IMG_Load("./sprites/entite/joueur/fixe_gauche.png");
  tab[DEP1]=IMG_Load("./sprites/entite/joueur/course1_gauche.png");
  tab[DEP2]=IMG_Load("./sprites/entite/joueur/course2_gauche.png");
  tab[DEP3]=IMG_Load("./sprites/entite/joueur/course3_gauche.png");
  tab[DEP4]=IMG_Load("./sprites/entite/joueur/course4_gauche.png");
  tab[DEP5]=IMG_Load("./sprites/entite/joueur/course5_gauche.png");
  tab[DEP6]=IMG_Load("./sprites/entite/joueur/course6_gauche.png");
  tab[DEP7]=IMG_Load("./sprites/entite/joueur/course7_gauche.png");
  tab[DEP8]=IMG_Load("./sprites/entite/joueur/course8_gauche.png");
  tab[JP1]=IMG_Load("./sprites/entite/joueur/saut1_gauche.png");
  tab[JP2]=IMG_Load("./sprites/entite/joueur/saut2_gauche.png");
  tab[JP3]=IMG_Load("./sprites/entite/joueur/saut3_gauche.png");
  tab[JP4]=IMG_Load("./sprites/entite/joueur/saut4_gauche.png");
  tab[JP5]=IMG_Load("./sprites/entite/joueur/saut5_gauche.png");
  tab[JP6]=IMG_Load("./sprites/entite/joueur/saut6_gauche.png");
  tab[JP7]=IMG_Load("./sprites/entite/joueur/saut7_gauche.png");
  tab[JP8]=IMG_Load("./sprites/entite/joueur/saut8_gauche.png");

  tab[NBSPRITES + SU]=IMG_Load("./sprites/entite/joueur/fixe.png");
  tab[NBSPRITES + DEP1]=IMG_Load("./sprites/entite/joueur/course1.png");
  tab[NBSPRITES + DEP2]=IMG_Load("./sprites/entite/joueur/course2.png");
  tab[NBSPRITES + DEP3]=IMG_Load("./sprites/entite/joueur/course3.png");
  tab[NBSPRITES + DEP4]=IMG_Load("./sprites/entite/joueur/course4.png");
  tab[NBSPRITES + DEP5]=IMG_Load("./sprites/entite/joueur/course5.png");
  tab[NBSPRITES + DEP6]=IMG_Load("./sprites/entite/joueur/course6.png");
  tab[NBSPRITES + DEP7]=IMG_Load("./sprites/entite/joueur/course7.png");
  tab[NBSPRITES + DEP8]=IMG_Load("./sprites/entite/joueur/course8.png");
  tab[NBSPRITES + JP1]=IMG_Load("./sprites/entite/joueur/saut1.png");
  tab[NBSPRITES + JP2]=IMG_Load("./sprites/entite/joueur/saut2.png");
  tab[NBSPRITES + JP3]=IMG_Load("./sprites/entite/joueur/saut3.png");
  tab[NBSPRITES + JP4]=IMG_Load("./sprites/entite/joueur/saut4.png");
  tab[NBSPRITES + JP5]=IMG_Load("./sprites/entite/joueur/saut5.png");
  tab[NBSPRITES + JP6]=IMG_Load("./sprites/entite/joueur/saut6.png");
  tab[NBSPRITES + JP7]=IMG_Load("./sprites/entite/joueur/saut7.png");
  tab[NBSPRITES + JP8]=IMG_Load("./sprites/entite/joueur/saut8.png");

}

/**
 * \brief Fonction d'affichage d'un sprite sur une zone
 *
 * @param zone la surface de destination pour l'affichage
 * @param sprite le sprite à afficher_surface
 * @param position les coordonnées de destination du sprite sur la zone
 */
void afficher_surface(SDL_Surface * zone, SDL_Surface * sprite, position_t position){
  SDL_Rect dest = {position.x, position.y};
  SDL_BlitSurface(sprite,NULL,zone,&dest);
}

/**
 * \brief Fonction de nettoyage d'une zone
 *
 * @param zone la surface à nettoyer
 */
void nettoyage_zone(SDL_Surface * zone){
  Uint32 noir = SDL_MapRGB(zone->format,0,0,0);
  SDL_FillRect(zone,NULL, noir);
}

/**
 * \brief Fonction de suppression d'un tableau de sprites
 *
 * @param tab tableau de sprites
 * @param nb_sprites nombre de sprites dans le tableau
 */
void supprimer_sprites(SDL_Surface ** tab, int nb_sprites){
  for(int i=0; i<nb_sprites; i++)
    SDL_FreeSurface(tab[i]);
}

/**
 * \brief Fonction de gestions des evenements
 *
 * @param fenetre la fenetre d'action
 * @return 0 si tout s'est bien déroulé (arrêt du programme)
 */
int evenements(SDL_Window * fenetre){
  SDL_Surface * surfaceFenetre = SDL_GetWindowSurface(fenetre);
  SDL_Event event;
  SDL_Surface * joueur[NBSPRITES*2];
  initialiser_sprites_personnages(joueur, NBSPRITES);
  position_t position={50,500};
  indSpritePer_t sprite_actuel=SU;
  int fullscreen=0;
  int saut_en_cours=0;
  int montee=0;
  int mouvement=0;
  int orientation=LEFT;
  int terminer=0;
  while(!terminer){
    nettoyage_zone(surfaceFenetre);
    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_QUIT: //Appui sur la croix quitte le programme
          terminer=1;
          break;
        case SDL_KEYUP:
          if(event.key.keysym.sym == SDLK_f){ //Appui sur F switch du mode plein écran au mode fenetré
            fullscreen=fullscreen_switch(fenetre,fullscreen);
            break;
          }
          else if(event.key.keysym.sym == SDLK_ESCAPE){//Appui sur Echap quitte le programme
            terminer=1;
            break;
          }
          else if(event.key.keysym.sym == SDLK_RIGHT){
            if(!saut_en_cours)
              sprite_actuel=SU;
            orientation=RIGHT;
            mouvement=0;
            break;
          }
          else if(event.key.keysym.sym == SDLK_LEFT){
            if(!saut_en_cours)
              sprite_actuel=SU;
            orientation=LEFT;
            mouvement=0;
            break;
          }
        case SDL_KEYDOWN:
          if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT){
            if(!saut_en_cours){
              sprite_actuel++;
              if(sprite_actuel > DEP8)
                sprite_actuel=DEP1;
            }
          }
          if(event.key.keysym.sym == SDLK_UP){
            if(!saut_en_cours){
              saut_en_cours=1;
              montee=2;
              sprite_actuel=DEP8;
            }
            break;
          }
          if(event.key.keysym.sym == SDLK_LEFT){
            if(position.x - VITESSE > 50)
              (position.x)-=VITESSE;
            mouvement=-1;
            orientation=LEFT;
            break;
          }
          else if(event.key.keysym.sym == SDLK_RIGHT){
            if(position.x + VITESSE < RES_H -50)
              (position.x)+=VITESSE;
            mouvement=1;
            orientation=RIGHT;
            break;
          }
        }
      }
      if(saut_en_cours && montee){
        if(mouvement==-1){
          if(position.x - VITESSE > 50)
            (position.x)-=VITESSE;
        }
        else if(mouvement==1){
          if(position.x + VITESSE < RES_H -50)
            (position.x)+=VITESSE;
        }
      }
      if(saut_en_cours==-1){
        if(montee==2){
          montee=0;
          sprite_actuel--;
        }
        else
          montee++;
        if(sprite_actuel==DEP8){
            saut_en_cours=0;
            sprite_actuel=SU;
            if (position.y + VITESSE < RES_V -50)
                (position.y)+=VITESSE;
        }
      if (position.y + VITESSE < RES_V -50)
          (position.y)+=VITESSE;
      }
      if(saut_en_cours==1){
        if(montee==2){
          montee=0;
          sprite_actuel++;
        }
        else
          montee++;
        if(position.y - VITESSE > 50)
          (position.y)-=VITESSE;
        if(sprite_actuel>JP8){
          sprite_actuel=JP8;
          saut_en_cours=-1;
          montee=2;
          if(position.y + VITESSE < RES_V -50)
            (position.y)+=VITESSE;
        }
      }
      afficher_surface(surfaceFenetre, joueur[sprite_actuel+orientation*NBSPRITES], position);
      SDL_UpdateWindowSurface(fenetre);
    }
    supprimer_sprites(joueur, NBSPRITES*2);
    SDL_FreeSurface(surfaceFenetre);
    quitter_SDL(fenetre);
    return 0;
  }
