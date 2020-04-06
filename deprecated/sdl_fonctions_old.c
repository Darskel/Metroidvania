#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../headers/sdl_fonctions.h"
#include "../headers/structs.h"


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

  fenetre = SDL_CreateWindow(NOM_JEU, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RES_H, RES_V, SDL_WINDOW_SHOWN);
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
 * \brief Fonction qui permet d'initialiser un tableau de sprites
 *
 * @param tab tableau de sprites à intialiser
 * @param taille la taille du tableau à initialiser
 */
void initialiser_sprites_personnages(personnage_t * p){

  p->sprites[SU]=IMG_Load("./sprites/entite/joueur/fixe_gauche.png");
  p->sprites[DEP1]=IMG_Load("./sprites/entite/joueur/course1_gauche.png");
  p->sprites[DEP2]=IMG_Load("./sprites/entite/joueur/course2_gauche.png");
  p->sprites[DEP3]=IMG_Load("./sprites/entite/joueur/course3_gauche.png");
  p->sprites[DEP4]=IMG_Load("./sprites/entite/joueur/course4_gauche.png");
  p->sprites[DEP5]=IMG_Load("./sprites/entite/joueur/course5_gauche.png");
  p->sprites[DEP6]=IMG_Load("./sprites/entite/joueur/course6_gauche.png");
  p->sprites[DEP7]=IMG_Load("./sprites/entite/joueur/course7_gauche.png");
  p->sprites[DEP8]=IMG_Load("./sprites/entite/joueur/course8_gauche.png");
  p->sprites[JP1]=IMG_Load("./sprites/entite/joueur/saut1_gauche.png");
  p->sprites[JP2]=IMG_Load("./sprites/entite/joueur/saut2_gauche.png");
  p->sprites[JP3]=IMG_Load("./sprites/entite/joueur/saut3_gauche.png");
  p->sprites[JP4]=IMG_Load("./sprites/entite/joueur/saut4_gauche.png");
  p->sprites[JP5]=IMG_Load("./sprites/entite/joueur/saut5_gauche.png");
  p->sprites[JP6]=IMG_Load("./sprites/entite/joueur/saut6_gauche.png");
  p->sprites[JP7]=IMG_Load("./sprites/entite/joueur/saut7_gauche.png");
  p->sprites[JP8]=IMG_Load("./sprites/entite/joueur/saut8_gauche.png");

  p->sprites[p->nb_sprites + SU]=IMG_Load("./sprites/entite/joueur/fixe.png");
  p->sprites[p->nb_sprites + DEP1]=IMG_Load("./sprites/entite/joueur/course1.png");
  p->sprites[p->nb_sprites + DEP2]=IMG_Load("./sprites/entite/joueur/course2.png");
  p->sprites[p->nb_sprites + DEP3]=IMG_Load("./sprites/entite/joueur/course3.png");
  p->sprites[p->nb_sprites + DEP4]=IMG_Load("./sprites/entite/joueur/course4.png");
  p->sprites[p->nb_sprites + DEP5]=IMG_Load("./sprites/entite/joueur/course5.png");
  p->sprites[p->nb_sprites + DEP6]=IMG_Load("./sprites/entite/joueur/course6.png");
  p->sprites[p->nb_sprites + DEP7]=IMG_Load("./sprites/entite/joueur/course7.png");
  p->sprites[p->nb_sprites + DEP8]=IMG_Load("./sprites/entite/joueur/course8.png");
  p->sprites[p->nb_sprites + JP1]=IMG_Load("./sprites/entite/joueur/saut1.png");
  p->sprites[p->nb_sprites + JP2]=IMG_Load("./sprites/entite/joueur/saut2.png");
  p->sprites[p->nb_sprites + JP3]=IMG_Load("./sprites/entite/joueur/saut3.png");
  p->sprites[p->nb_sprites + JP4]=IMG_Load("./sprites/entite/joueur/saut4.png");
  p->sprites[p->nb_sprites + JP5]=IMG_Load("./sprites/entite/joueur/saut5.png");
  p->sprites[p->nb_sprites + JP6]=IMG_Load("./sprites/entite/joueur/saut6.png");
  p->sprites[p->nb_sprites + JP7]=IMG_Load("./sprites/entite/joueur/saut7.png");
  p->sprites[p->nb_sprites + JP8]=IMG_Load("./sprites/entite/joueur/saut8.png");

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
 * \brief Fonction d'initialisation de la structure personnage
 *
 * @param personnage pointeur sur la structure à initialiser
 * @return le pointeur sur la structure initialisée
 */
personnage_t * initialisation_personnage(){
  personnage_t * personnage=malloc(sizeof(personnage_t));
  personnage->vit_dep=2;
  personnage->pos.x=100;
  personnage->pos.y=500;
  personnage->delta.delta_x.numerateur=0;
  personnage->delta.delta_x.denominateur=1;
  personnage->delta.delta_y.numerateur=0;
  personnage->delta.delta_y.denominateur=1;
  personnage->spriteActuel=SU;
  personnage->nb_sprites=17;
  SDL_Surface ** sprites=malloc(sizeof(SDL_Surface *)*(personnage->nb_sprites*2));
  personnage->sprites=sprites;
  initialiser_sprites_personnages(personnage);
  return personnage;
}

/**
 * \brief Fonction d'initialisation de la salle
 *
 * @param s la structure salle à initialiser
 */
void initialiser_salle(salle_t * s){

}

/**
 * \brief Fonction d'initialisation des sprites de la salle
 *
 * @param s la structure salle où stocker les sprites
 */
void initialiser_sprites_salle(salle_t * s){

}

/**
 * \brief Fonction d'affichage de la salle
 *
 * @param zone la zone d'test_affichage_salle
 * @param salle la structure salle à afficher
 */
void afficher_salle(SDL_Surface * zone, salle_t * salle){

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
  personnage_t * personnage=initialisation_personnage();
  int nbspritestemp=0;
	Uint32 frameStart;
	int frameTime;
	int tempsSprite = 0;
  int tempsSaut=0;
  boolean_t Gauche=FALSE;
  boolean_t Droite=FALSE;
  boolean_t Saut=FALSE;
  boolean_t Tombe=FALSE;
  //boolean_t Tire=FALSE;

  int fin=0;
  while(!fin){
		frameStart = SDL_GetTicks();
		nettoyage_zone(surfaceFenetre);
		  while(SDL_PollEvent(&event)){
		    switch(event.type){
		      case SDL_QUIT: //Appui sur la croix quitte le programme
		        fin=1;
		        break;
		      case SDL_KEYUP:
		        switch(event.key.keysym.sym){
		          case SDLK_ESCAPE://Appui sur Echap quitte le programme
		            fin=1;
		            break;
							case SDLK_LEFT:
  		            Gauche=FALSE;
  								nbspritestemp=0;
  		            if(!Saut&&!Tombe){
  		              personnage->spriteActuel=SU;
                  }
		            break;
		          case SDLK_RIGHT:
  		            Droite=FALSE;
  								nbspritestemp=personnage->nb_sprites;
  		            if(!Saut&&!Tombe){
  		              personnage->spriteActuel=SU+nbspritestemp;
  								}
		            break;
		        }
		        break;
		      case SDL_KEYDOWN:
		        switch(event.key.keysym.sym){
		          case SDLK_UP:
		            if(!Saut&&!Tombe){
		              Saut=TRUE;
                  tempsSaut=TEMPSSAUT;
                }
		            break;
		          case SDLK_LEFT:
                if(!Saut&&!Tombe)
                  Gauche=TRUE;
		            break;
		          case SDLK_RIGHT:
                if(!Saut&&!Tombe)
                  Droite=TRUE;
		            break;
		        }
		        break;
		      }
		    }
        if((!Droite&&!Gauche&&!Saut&&!Tombe))
          personnage->spriteActuel=SU+nbspritestemp;
		    if(Droite){
		      if(personnage->pos.x + personnage->vit_dep < RES_H - LIMITS){
		        (personnage->pos.x)+=personnage->vit_dep;
		      }
          if(!Saut && !Tombe && !Gauche){
            nbspritestemp=personnage->nb_sprites;
            if(tempsSprite==0){
  		      	if(personnage->spriteActuel<DEP1+nbspritestemp||personnage->spriteActuel>DEP8+nbspritestemp)
  		        	personnage->spriteActuel=DEP1+nbspritestemp;
  		      	else if (personnage->spriteActuel>=DEP1+nbspritestemp && personnage->spriteActuel<=DEP8+nbspritestemp)
  							personnage->spriteActuel++;
  					}
          }
		    }

		   if(Gauche){
		      if(personnage->pos.x - personnage->vit_dep > LIMITS){
		        (personnage->pos.x)-=personnage->vit_dep;
		      }
          if(!Saut && !Tombe && !Droite){
            nbspritestemp=0;
            if(tempsSprite==0){
  		      	if(personnage->spriteActuel<DEP1||personnage->spriteActuel>DEP8)
  		        	personnage->spriteActuel=DEP1;
  		      	else if (personnage->spriteActuel>=DEP1 && personnage->spriteActuel<=DEP8)
  							personnage->spriteActuel++;
  					}
		      }
        }

		    if(Tombe){
					if(personnage->pos.y + personnage->vit_dep < RES_V - LIMITS){
				    (personnage->pos.y)+=personnage->vit_dep;
					}
          personnage->spriteActuel=JP8+nbspritestemp;
          tempsSaut--;
          if(tempsSaut==(-1*TEMPSSAUT)){
             Saut=FALSE;
             Tombe=FALSE;
             personnage->spriteActuel=SU+nbspritestemp;
          }
		    }

		    else if(Saut){
					if(personnage->pos.y - personnage->vit_dep > LIMITS){
						(personnage->pos.y)-=personnage->vit_dep;
					}
					if(tempsSprite==0){
						if(personnage->spriteActuel>=JP1+nbspritestemp && personnage->spriteActuel<JP8+nbspritestemp)
								personnage->spriteActuel++;
						else if(!(personnage->spriteActuel>=JP1+nbspritestemp && personnage->spriteActuel<=JP8+nbspritestemp))
						    personnage->spriteActuel=JP1+nbspritestemp;
					}
          tempsSaut--;
          if(tempsSaut==0){
             Saut=FALSE;
             Tombe=TRUE;
          }
		     }

				if(tempsSprite>=TEMPSSPRITE)
					tempsSprite=0;
				else tempsSprite++;
				fprintf(stdout,"%d\n",personnage->spriteActuel);
		    afficher_surface(surfaceFenetre, personnage->sprites[personnage->spriteActuel], personnage->pos);
		    SDL_UpdateWindowSurface(fenetre);
				frameTime = SDL_GetTicks() - frameStart;
				if(frameTime < FRAMEDELAY){
					SDL_Delay(FRAMEDELAY - frameTime);
				}
    }
    supprimer_sprites(personnage->sprites, personnage->nb_sprites*2);
    free(personnage->sprites);
    free(personnage);
    SDL_FreeSurface(surfaceFenetre);
    quitter_SDL(fenetre);
    return 0;
  }
