#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "constantes_SDL.h"
#include "structs.h"

#define NBSPRITES 17

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

SDL_Surface ** initialiser_sprites_personnages(SDL_Surface ** tab, int taille, char* nom){
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
  tab[0]=IMG_Load("sprites/entite/joueur/SU.png");
  return tab;
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
  SDL_Surface * joueur[NBSPRITES];
  initialiser_sprites_personnages(joueur, NBSPRITES, "joueur");
  position_t position={50,50};
  indSpritePer_t sprite_actuel=SU;
  int fullscreen=0;
  int terminer=0;
  while(1){
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
        case SDL_KEYDOWN:
          if(event.key.keysym.sym == SDLK_LEFT){
            if(position.x - 1 > 50)
              (position.x)--;
            break;
          }
          else if(event.key.keysym.sym == SDLK_RIGHT){
            if(position.x + 1 < RES_H -50)
              (position.x)++;
            break;
          }
          else if(event.key.keysym.sym == SDLK_DOWN){
            if(position.y - 1 < RES_V -50)
              (position.y)++;
            break;
          }
          else if(event.key.keysym.sym == SDLK_UP){
            if(position.x + 1 > 50)
              (position.y)--;
            break;
          }
        }
      }
      if(terminer) break;
      afficher_surface(surfaceFenetre, joueur[sprite_actuel], position);
      SDL_UpdateWindowSurface(fenetre);
    }
    supprimer_sprites(joueur, NBSPRITES);
    SDL_FreeSurface(surfaceFenetre);
    quitter_SDL(fenetre);
    return 0;
  }
