#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../headers/constantes_SDL.h"
#include "../headers/structs.h"

#define NBSPRITES 17
#define VITESSE 2

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

void quitter_SDL(SDL_Window * fenetre){
  SDL_DestroyWindow(fenetre);
  SDL_Quit();
}

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

void initialiser_sprites_personnages(SDL_Surface ** tabD, SDL_Surface ** tabG, int taille){
  /*char * dir = malloc(sizeof(char)*(16+strlen(nom)+1));
  char * spr = malloc(sizeof(char)*(20+strlen(nom)+1));
  dir= "sprites/entite/";
  strcat(dir, nom);
  strcat(dir, "/");

  char * images[]={"SU.png","DEP1.png","DEP2.png","DEP3.png","DEP4.png","DEP5.png","DEP6.png","DEP7.png","DEP8.png","JP1.png","JP2.png","JP3.png","JP4.png","JP5.png","JP6.png","JP7.png","JP8.png"};

  for(int i=0; i<taille; i++){
    strcpy(spr,dir);
    strcat(spr,images[i]);
    tab[i]=IMG_Load(spr);
  }
  free(dir);
  free(spr);*/
  tabD[SU]=IMG_Load("./sprites/entite/joueur/fixe.png");
  tabD[DEP1]=IMG_Load("./sprites/entite/joueur/course1.png");
  tabD[DEP2]=IMG_Load("./sprites/entite/joueur/course2.png");
  tabD[DEP3]=IMG_Load("./sprites/entite/joueur/course3.png");
  tabD[DEP4]=IMG_Load("./sprites/entite/joueur/course4.png");
  tabD[DEP5]=IMG_Load("./sprites/entite/joueur/course5.png");
  tabD[DEP6]=IMG_Load("./sprites/entite/joueur/course6.png");
  tabD[DEP7]=IMG_Load("./sprites/entite/joueur/course7.png");
  tabD[DEP8]=IMG_Load("./sprites/entite/joueur/course8.png");
  tabD[JP1]=IMG_Load("./sprites/entite/joueur/saut1.png");
  tabD[JP2]=IMG_Load("./sprites/entite/joueur/saut2.png");
  tabD[JP3]=IMG_Load("./sprites/entite/joueur/saut3.png");
  tabD[JP4]=IMG_Load("./sprites/entite/joueur/saut4.png");
  tabD[JP5]=IMG_Load("./sprites/entite/joueur/saut5.png");
  tabD[JP6]=IMG_Load("./sprites/entite/joueur/saut6.png");
  tabD[JP7]=IMG_Load("./sprites/entite/joueur/saut7.png");
  tabD[JP8]=IMG_Load("./sprites/entite/joueur/saut8.png");

  tabG[SU]=IMG_Load("./sprites/entite/joueur/fixe_gauche.png");
  tabG[DEP1]=IMG_Load("./sprites/entite/joueur/course1_gauche.png");
  tabG[DEP2]=IMG_Load("./sprites/entite/joueur/course2_gauche.png");
  tabG[DEP3]=IMG_Load("./sprites/entite/joueur/course3_gauche.png");
  tabG[DEP4]=IMG_Load("./sprites/entite/joueur/course4_gauche.png");
  tabG[DEP5]=IMG_Load("./sprites/entite/joueur/course5_gauche.png");
  tabG[DEP6]=IMG_Load("./sprites/entite/joueur/course6_gauche.png");
  tabG[DEP7]=IMG_Load("./sprites/entite/joueur/course7_gauche.png");
  tabG[DEP8]=IMG_Load("./sprites/entite/joueur/course8_gauche.png");
  tabG[JP1]=IMG_Load("./sprites/entite/joueur/saut1_gauche.png");
  tabG[JP2]=IMG_Load("./sprites/entite/joueur/saut2_gauche.png");
  tabG[JP3]=IMG_Load("./sprites/entite/joueur/saut3_gauche.png");
  tabG[JP4]=IMG_Load("./sprites/entite/joueur/saut4_gauche.png");
  tabG[JP5]=IMG_Load("./sprites/entite/joueur/saut5_gauche.png");
  tabG[JP6]=IMG_Load("./sprites/entite/joueur/saut6_gauche.png");
  tabG[JP7]=IMG_Load("./sprites/entite/joueur/saut7_gauche.png");
  tabG[JP8]=IMG_Load("./sprites/entite/joueur/saut8_gauche.png");

}

void afficher_surface(SDL_Surface * zone, SDL_Surface * sprite, position_t position){
  SDL_Rect dest = {position.x, position.y};
  SDL_BlitSurface(sprite,NULL,zone,&dest);
}

void nettoyage_zone(SDL_Surface * zone){
  Uint32 noir = SDL_MapRGB(zone->format,0,0,0);
  SDL_FillRect(zone,NULL, noir);
}

void supprimer_sprites(SDL_Surface ** tab, int nb_sprites){
  for(int i=0; i<nb_sprites; i++)
    SDL_FreeSurface(tab[i]);
}


int evenements(SDL_Window * fenetre){
  SDL_Surface * surfaceFenetre = SDL_GetWindowSurface(fenetre);
  SDL_Event event;
  SDL_Surface * joueurD[NBSPRITES];
  SDL_Surface * joueurG[NBSPRITES];
  initialiser_sprites_personnages(joueurD, joueurG, NBSPRITES);
  position_t position={50,500};
  indSpritePer_t sprite_actuel=SU;
  int fullscreen=0;
  int saut_en_cours=0;
  int montee=0;
  int mouvement=0;
  int orientation=1;
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
          else if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT){
            if(!saut_en_cours)
              sprite_actuel=SU;
            orientation=mouvement;
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
            orientation=mouvement;
            break;
          }
          else if(event.key.keysym.sym == SDLK_RIGHT){
            if(position.x + VITESSE < RES_H -50)
              (position.x)+=VITESSE;
            mouvement=1;
            orientation=mouvement;
            break;
          }
        }
      }
      if(saut_en_cours){
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
      if(orientation==1)
        afficher_surface(surfaceFenetre, joueurD[sprite_actuel], position);
      else if(orientation==-1)
        afficher_surface(surfaceFenetre, joueurG[sprite_actuel], position);
      SDL_UpdateWindowSurface(fenetre);
    }
    supprimer_sprites(joueurD, NBSPRITES);
    supprimer_sprites(joueurG, NBSPRITES);
    SDL_FreeSurface(surfaceFenetre);
    quitter_SDL(fenetre);
    return 0;
  }
