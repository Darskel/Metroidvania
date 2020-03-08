#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../headers/constantes_SDL.h"
#include "../headers/structs.h"

#define NBSPRITES 17
#define VITESSE 1

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

/*
  Peut être que pour cette fonction tu pourrais utiliser la fonction
  SDL_SetWindowFullscreen : https://wiki.libsdl.org/SDL_SetWindowFullscreen
  avec les flags qui vont avec : https://wiki.libsdl.org/SDL_WindowFlags
  et ici la def d'une struct SDL_Window:
  struct SDL_Window
  {
    const void *magic;
    Uint32 id;
    char *title;
    SDL_Surface *icon;
    int x, y;
    int w, h;
    int min_w, min_h;
    int max_w, max_h;
    Uint32 flags;
    Uint32 last_fullscreen_flags;
    */
    /* Stored position and size for windowed mode */
    /*
    SDL_Rect windowed;

    SDL_DisplayMode fullscreen_mode;

    float brightness;
    Uint16 *gamma;
    Uint16 *saved_gamma;        */

    /* (just offset into gamma) */

    /*

    SDL_Surface *surface;
    SDL_bool surface_valid;

    SDL_bool is_hiding;
    SDL_bool is_destroying;

    SDL_WindowShaper *shaper;

    SDL_HitTest hit_test;
    void *hit_test_data;

    SDL_WindowUserData *data;

    void *driverdata;

    SDL_Window *prev;
    SDL_Window *next;
  };
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

void initialiser_sprites_personnages(SDL_Surface ** tab, int taille){
  /*char * dir = malloc(sizeof(char)*(15+strlen(nom)+1));
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

  /*
    Pourquoi ne pas faire comme au dessus ?

    char spr[50];
    char dir[] = "./sprites/entite/joueur/";
    strcat(spr,dir);
    char images[NBSPRITES*2][] = {
      "fixe.png",
      "course1.png",
      "course2.png",
      "course3.png",
      "course4.png",
      "course5.png",
      "course6.png",
      "course7.png",
      "course8.png",
      "saut1.png",
      "saut2.png",
      "saut3.png",
      "saut4.png",
      "saut5.png",
      "saut6.png",
      "saut7.png",
      "saut8.png",
      "fixe_gauche.png",
      "course1_gauche.png",
      "course2_gauche.png",
      "course3_gauche.png",
      "course4_gauche.png",
      "course5_gauche.png",
      "course6_gauche.png",
      "course7_gauche.png",
      "course8_gauche.png",
      "saut1_gauche.png",
      "saut2_gauche.png",
      "saut3_gauche.png",
      "saut4_gauche.png",
      "saut5_gauche.png",
      "saut6_gauche.png",
      "saut7_gauche.png",
      "saut8_gauche.png",
    }
    for(int i = 0; i < NBSPRITES*2; i++){
      strcat(spr,images[i]);
      tab[i]=IMG_Load(spr);
    }

    Sinon laisse comme ça mais fais un seul tableau, c'est plus efficace et tu pourras
    faire orientation = LEFT (0) et orientation = RIGHT (1) puis quand tu choisis le
    sprite tu fais tab[sprite_actuel + orientation * NBSPRITES] pour avoir le bon sprite
  */

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

void afficher_surface(SDL_Surface * zone, SDL_Surface * sprite, position_t position){
  SDL_Rect dest = {position.x, position.y};
  SDL_BlitSurface(sprite,NULL,zone,&dest);
}

void nettoyage_zone(SDL_Surface * zone){
  Uint32 noir = SDL_MapRGB(zone->format,0,0,0);
  SDL_FillRect(zone,NULL, noir);
}

/*
  Tu as défini une constante pour le nombre de sprites, pourquoi tu ne l'utilises pas ?
*/
void supprimer_sprites(SDL_Surface ** tab, int nb_sprites){
  for(int i=0; i<nb_sprites; i++)
    SDL_FreeSurface(tab[i]);
}


int evenements(SDL_Window * fenetre){
  SDL_Surface * surfaceFenetre = SDL_GetWindowSurface(fenetre);
  SDL_Event event;
  SDL_Surface * joueur[NBSPRITES*2];
  initialiser_sprites_personnages(joueur, NBSPRITES);
  position_t position={50,500};
  //Pourquoi utilises tu la définition de indSpritePer_t et pas la structure joueur ?
  indSpritePer_t sprite_actuel=SU;
  int fullscreen=0; // doit pouvoir être géré directement avec SDL_Window (voir dit précédemment)
  int saut_en_cours=0; // Un saut est en cour si sprite_actuel >= JP1 && sprite_actuel <= JP8
  int montee=0;
  int mouvement=0;
  int orientation=LEFT; //existe déjà dans la structure joueur du coup
  int terminer=0; //on préfère utiliser un nom plutot qu'un verbe pour une variable (Alive est le plus courrant dans ces conditions)
  while(!terminer){
    nettoyage_zone(surfaceFenetre);
    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_QUIT: //Appui sur la croix quitte le programme
          terminer=1;
          break;
        case SDL_KEYUP:
          /*
            Pourquoi des if-elseif ? pourquoi pas un switch encore une fois ?
            switch(event.key.keysym.sym){
              case SDLK_f: //code
                break;
              case SDLK_ESCAPE: terminer = 1;
                brea;
              case SDLK_RIGHT: //ici est géré le ou (||)
              case SDLK_LEFT: //code
                break;
            }
          */
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
          /*
            Pareil que pour keyup
          */
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
      /*
        Je comprends plus rien à partir d'ici
      */
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
      //peut être simplier avec un seul tableau (voir dit précédemment)
      afficher_surface(surfaceFenetre, joueur[sprite_actuel+orientation*NBSPRITES], position);
      SDL_UpdateWindowSurface(fenetre);
    }
    supprimer_sprites(joueur, NBSPRITES*2);
    SDL_FreeSurface(surfaceFenetre);
    quitter_SDL(fenetre);
    return 0;
  }
