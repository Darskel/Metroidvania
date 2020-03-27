#include "../headers/sdl_fonctions.h"

/**
 * \file sdl_fonctions.c
 * \brief Fichier qui regroupent les fonctions utilisées pour la gestion des graphismes et des évènements (SDL2)
 * \author Marie-Nina MUNAR L2 Info Le Mans
 * \version 3.0
 * \date 26/03/2020
*/

/**
 * \brief Fonction d'initalisation de la SDL et de création de la fenetre et du renderer ainsi que du remplissage du mode d'écran (en parametre)
 *
 * @param fenetre pointeur sur un pointeur de SDL_Window qui correspond à la fenêtre initialiser
 * @param renderer pointeur sur un pointeur de SDL_Renderer qui correspond au renderer à initialiser
 * @param mode pointeur sur un SDL_DisplayMode qui va contenir les informations sur l'écran
 * @param fullscreen entier qui vaut 0 si on souhaite créer une fenetre simple et 1 si on la veut en plein écran
 */
void initialisation_SDL(SDL_Window ** fenetre, SDL_Renderer ** renderer, SDL_DisplayMode * mode, boolean_t fullscreen){

  int res_h;
  int res_v;
  SDL_Surface * icon;

  if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
    fprintf(stderr, "Echec de l'initalisation de la SDL (%s)\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  if(TTF_Init () ==  -1){
    fprintf(stderr, "Erreur d’initialisation de TTF_Init : %s\n", TTF_GetError ());
    exit(EXIT_FAILURE);
  }

  if(SDL_GetDesktopDisplayMode(0, mode)){
    fprintf(stderr, "Echec de la récupération des infos de l'écran (%s)\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  res_h=mode->w;
  res_v=mode->h;

  *fenetre = SDL_CreateWindow(NOM_JEU, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, res_h, res_v, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_SHOWN);
  if(!(*fenetre)){
    fprintf(stderr, "Erreur de creation de la fenetre : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  icon=IMG_Load("./sprites/autre/icon.png");
  SDL_SetWindowIcon(*fenetre,icon);

  *renderer = SDL_CreateRenderer(*fenetre, -1, SDL_RENDERER_ACCELERATED);
  if(!(*renderer)){
    fprintf(stderr, "Erreur de creation du renderer : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  SDL_SetRenderDrawColor(*renderer,0,0,0,255);
  SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);

}

/**
 * \brief Fonction qui permet de détruire la fenetre et quitter la SDL
 *
 * @param fenetre le pointeur sur le pointeur de la fenetre SDL_Window à détruire
 * @param renderer le pointeur sur le pointeur de la renderer SDL_Renderer à détruire
 */
void quitter_SDL(SDL_Window ** fenetre, SDL_Renderer ** renderer){
  SDL_DestroyRenderer(*renderer);
  SDL_DestroyWindow(*fenetre);
  SDL_Quit();
}

/**
 * \brief Fonction de gestions des evenements
 *
 * @param fenetre la fenetre d'action
 * @return 0 si tout s'est bien déroulé (arrêt du programme)
 */
  void evenements(SDL_Window * fenetre, SDL_Renderer * renderer, SDL_DisplayMode * mode){
    SDL_Event event;
  	Uint32 frameStart;
  	int frameTime;
    int mousex;
    int mousey;
    int fin=0;
    while(!fin){
  		frameStart = SDL_GetTicks();
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
  	        }
  	        break;
  	      case SDL_KEYDOWN:
  	        switch(event.key.keysym.sym){
  	        }
  	        break;
          case SDL_MOUSEMOTION :
            mousex=event.motion.x;
            mousey=event.motion.y;
  	      }
  	    }
  			frameTime = SDL_GetTicks() - frameStart;
  			if(frameTime < FRAMEDELAY){
  				SDL_Delay(FRAMEDELAY - frameTime);
  			}
      }
  }

/**
 * \brief Fonction qui permet d'initialiser une texture à partir d'une image
 *
 * @param path chaine de caracteres qui correspond au chemin de l'image
 * @param renderer le pointeur sur SDL_Renderer
 * @param access le type d'accès à la texture
 * @return le pointeur sur la texture générée
 */
SDL_Texture * initialiser_texture(char * path, SDL_Renderer * renderer, SDL_TextureAccess access){
  SDL_Surface *surface = NULL;
  SDL_Texture *texture, *tmp = NULL;
  surface = IMG_Load(path);
  if(surface==NULL){
      fprintf(stderr, "Erreur IMG_LOAD pour %s", path);
      exit(EXIT_FAILURE);
  }
  tmp = SDL_CreateTextureFromSurface(renderer, surface);
  if(tmp==NULL){
      fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
      exit(EXIT_FAILURE);
  }
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                              access, surface->w, surface->h);
  if(texture==NULL){
      fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
      exit(EXIT_FAILURE);
  }
  SDL_SetRenderTarget(renderer, texture); /* La cible de rendu est maintenant texture. */
  SDL_RenderCopy(renderer, tmp, NULL, NULL); /* On copie tmp sur texture */
  SDL_DestroyTexture(tmp);
  SDL_FreeSurface(surface);
  SDL_SetRenderTarget(renderer, NULL); /* La cible de rendu est de nouveau le renderer. */
  return texture;
}

/**
 * \brief Fonction d'initialisation de la structure personnage
 *
 * @param personnage pointeur sur la structure à initialiser
 * @return le pointeur sur la structure initialisée
 */
personnage_t * initialisation_personnage(SDL_Renderer * renderer){
  personnage_t * personnage=malloc(sizeof(personnage_t));
  personnage->pv=1;
  personnage->vit_dep=2;
  personnage->vit_att=1;
  personnage->pos.x=0;
  personnage->pos.y=0;
  personnage->delta.x=0;
  personnage->delta.y=0;
  SDL_Texture * texture=initialiser_texture(PLAYERSPRITESPATH, renderer, SDL_TEXTUREACCESS_STREAMING);
  personnage->sprites=texture;
  personnage->spriteActuel.x=0;
  personnage->spriteActuel.y=0;
  personnage->spriteActuel.h=33;
  personnage->spriteActuel.w=30;
  int * nbAnim = malloc(4*sizeof(int));
  nbAnim[0]=1;
  nbAnim[1]=8;
  nbAnim[2]=8;
  nbAnim[3]=3;
  personnage->nbAnim=nbAnim;
  personnage->forme='h';
  return personnage;
}

/**
 * \brief Fonction de destruction de la structure personnage
 *
 * @param personnage pointeur sur le pointeur de la structure à détruire
 */
void destroy_personnage(personnage_t ** personnage){

  SDL_DestroyTexture((*personnage)->sprites);
  free((*personnage)->nbAnim);
  free(*personnage);
  *personnage=NULL;
}

/**
 * \brief Fonction d'initialisation de la salle
 *
 * @param nomFichier une chaine de caracteres qui contient le nom du fichier de la salle
 * @return un pointeur sur la structure salle initalisée
 */
salle_t * initialiser_salle(SDL_Renderer * renderer, char* nomFichier, SDL_Texture * tileset){
  salle_t ** salle=malloc(sizeof(salle_t *));
  *salle=NULL;
  lireSalle(nomFichier, salle);
  (*salle)->background=initialiser_texture("./sprites/salles/salle_entree_grotte.png", renderer, SDL_TEXTUREACCESS_STATIC);
  (*salle)->tileset=tileset;
  return (*salle);
}

/**
 * \brief Fonction de destruction de la structure personnage
 *
 * @param salle pointeur sur le pointeur de la structure à détruire
 */
void destroy_salle(salle_t ** salle){
  SDL_DestroyTexture((*salle)->background);
  nettoyerSalle(salle);
}

/**
 * \brief Fonction d'affichage de la salle
 *
 * @param renderer le pointeur vers le SDL_Renderer à utiliser
 * @param salle la structure salle à afficher
 */
void afficher_salle(SDL_Renderer * renderer, salle_t * salle){

}


/**
 * \brief Fonction d'affichage du personnage
 *
 * @param renderer le pointeur vers le SDL_Renderer à utiliser
 * @param personnage la structure personnage à afficher
 */
void afficher_personnage(SDL_Renderer * renderer, personnage_t * personnage){

}
