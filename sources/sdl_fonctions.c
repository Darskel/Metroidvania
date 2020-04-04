#include "../headers/sdl_fonctions.h"

/**
 * \file sdl_fonctions.c
 * \brief Fichier qui regroupent les fonctions utilisées pour la gestion des graphismes et des évènements (SDL2)
 * \author Marie-Nina MUNAR L2 Info Le Mans
 * \version 4.1
 * \date 04/04/2020
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
  /*if(TTF_Init () ==  -1){
    fprintf(stderr, "Erreur d’initialisation de TTF_Init : %s\n", TTF_GetError ());
    exit(EXIT_FAILURE);
  }*/

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

  icon=IMG_Load("./sprites/autre/icone.png");
  SDL_SetWindowIcon(*fenetre,icon);

  *renderer = SDL_CreateRenderer(*fenetre, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE);
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
  void evenements(SDL_Renderer * renderer, SDL_DisplayMode * mode){
    SDL_Event event;

    int mousex;
    int mousey;
    Sint16 x_move;
    Sint16 y_move;

    SDL_Joystick* pJoystick;
    int numJoystick = SDL_NumJoysticks();
    if ( numJoystick >= 1 )
    {
        pJoystick = SDL_JoystickOpen(0);
        if ( pJoystick == NULL )
        {
            fprintf(stderr,"Erreur pour ouvrir le premier joystick\n");
        }
        else{
          printf("WARNING : Programme conçu pour manette XBOX\n");
          SDL_JoystickEventState(SDL_ENABLE);
        }
    }
    boolean_t Gauche = FALSE;
    boolean_t Droite = FALSE;
    boolean_t fin=FALSE;
    SDL_Texture * tileset;
    salle_t * salle;
    personnage_t * perso;
    position_t positionDepart;
    position_t positionDepartDelta;

    tileset=initialiser_texture(TILESETPATH, renderer);
    salle=initialiser_salle(renderer, NIVEAUTXT, tileset);

    positionDepart.x = 1;
    positionDepartDelta.x = 0;
    positionDepart.y = salle->hauteur - HAUTEURHITBOXPERS/TAILLEBLOC -2;
    positionDepartDelta.y = 7;

    perso=initialisation_personnage(renderer, positionDepart, positionDepartDelta);

    while(!fin){
  	  while(SDL_PollEvent(&event)){
  	    switch(event.type){
  	      case SDL_QUIT: //Appui sur la croix quitte le programme
  	        fin=TRUE;
  	        break;
  	      case SDL_KEYUP:
  	        switch(event.key.keysym.sym){
  	          case SDLK_ESCAPE://Appui sur Echap quitte le programme
  	            fin=TRUE;
  	            break;
              case SDLK_LEFT:
                Gauche=FALSE;
                break;
              case SDLK_RIGHT:
                Droite=FALSE;
                break;
  	        }
  	        break;
  	      case SDL_KEYDOWN:
  	        switch(event.key.keysym.sym){
              case SDLK_LEFT:
                Gauche=TRUE;
                break;
              case SDLK_RIGHT:
                Droite=TRUE;
                break;
  	        }
  	        break;
          case SDL_MOUSEMOTION :
            mousex=event.motion.x;
            mousey=event.motion.y;
            break;
          case SDL_JOYBUTTONDOWN :
          case SDL_JOYBUTTONUP :
            break;
          case SDL_JOYAXISMOTION :
            switch(event.jaxis.axis){
              case 0 :
                if(event.jaxis.value>ZONEMORTE){
                  Droite = TRUE;
                }
                else if(event.jaxis.value<ZONEMORTE*-1){
                  Gauche = TRUE;
                }
                else{
                  Gauche = FALSE;
                  Droite = FALSE;
                }
                break;
            }
            break;
          case SDL_JOYBALLMOTION :
            break;
          case SDL_JOYHATMOTION :
            switch(event.jhat.value){
              case SDL_HAT_CENTERED :
                Gauche = FALSE;
                Droite = FALSE;
                break;
              case SDL_HAT_LEFT :
                Gauche = TRUE;
                break;
              case SDL_HAT_RIGHT :
                Droite = TRUE;
                break;
            }
  	      }
  	    }
        if ( pJoystick != NULL ){
          x_move = SDL_JoystickGetAxis(pJoystick, 0);
          y_move = SDL_JoystickGetAxis(pJoystick, 1);
        }

        if((Gauche&&Droite)){
          Gauche=FALSE;
          Droite=FALSE;
        }

        if((!Gauche&&!Droite))
          perso->etat=IDLE;

        else if(Gauche){
          depGauche(perso, salle);
          perso->direction = LEFT;
        }

        else if(Droite){
          depDroite(perso, salle);
          perso->direction = RIGHT;
        }

        miseAjourSprites(perso);
        affichage_complet(renderer, salle, perso);
      }
      destroy_salle(&salle);
      destroy_personnage(&perso);
      if(pJoystick != NULL)
        SDL_JoystickClose(pJoystick);
  }

/**
 * \brief Fonction qui permet d'initialiser une texture à partir d'une image
 *
 * @param path chaine de caracteres qui correspond au chemin de l'image
 * @param renderer le pointeur sur SDL_Renderer
 * @return le pointeur sur la texture générée
 */
SDL_Texture * initialiser_texture(char * path, SDL_Renderer * renderer){
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
  SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_TARGET, surface->w, surface->h);
  if(texture==NULL){
      fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
      exit(EXIT_FAILURE);
  }
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
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
personnage_t * initialisation_personnage(SDL_Renderer * renderer, position_t positionDepart, position_t positionDepartDelta){
  personnage_t * personnage=malloc(sizeof(personnage_t));
  personnage->pv=1;
  personnage->pv_max=1;
  personnage->inv=0;
  personnage->direction=RIGHT;
  personnage->vit_dep=VITDEPPERS;
  personnage->vit_att=VITATTACKPERS;
  personnage->pos=positionDepart;
  personnage->delta=positionDepartDelta;
  SDL_Texture * texture=initialiser_texture(PLAYERSPRITESPATH, renderer);
  personnage->sprites=texture;
  personnage->spriteActuel.x=0;
  personnage->spriteActuel.y=0;
  personnage->spriteActuel.h=HAUTEURSPRITEPERS;
  personnage->spriteActuel.w=LARGEURSPRITEPERS;
  personnage->hitbox.hauteur=HAUTEURHITBOXPERS;
  personnage->hitbox.largeur=LARGEURHITBOXPERS;
  personnage->posxhitbox = positionDepart.x * TAILLEBLOC + positionDepartDelta.x + OFFSETHITBOX;
  personnage->etat = IDLE;
  personnage->newEtat = FALSE;
  personnage->evoSprite = 0;
  int * nbAnim = malloc(4*sizeof(int));
  nbAnim[0]=1;
  nbAnim[1]=8;
  nbAnim[2]=8;
  nbAnim[3]=3;
  personnage->nbAnim=nbAnim;
  personnage->forme='h';
  for(int i = 0; i < TAILLE_INVENTAIRE; i++)
        personnage->inventaire[i] = 0;
  personnage->nomObj[0] = "champignon";
  personnage->nomObj[1] = "cle bleue";
  personnage->nomObj[2] = "cle rouille";
  personnage->nomObj[3] = "cle rouge";
  personnage->nomObj[4] = "cle verte";
  personnage->nomObj[5] = "double saut";
  personnage->nomObj[6] = "fleche de feu";
  personnage->nomObj[7] = "renard";
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
  char nom_bg[100];

  strcpy(nom_bg,DIRBG);
  strcat(nom_bg, nomFichier);
  nom_bg[strlen(nom_bg) - 3] = '\0';
  strcat(nom_bg, "png");

  lireSalle(nomFichier, salle);
  (*salle)->background=initialiser_texture(nom_bg, renderer);
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
 * \brief Fonction d'affichage de la salle (fait en coop avec Yannis Allain)
 *
 * @param renderer le pointeur vers le SDL_Renderer à utiliser
 * @param salle la structure salle à afficher
 */
void afficher_salle(SDL_Renderer * renderer, salle_t * salle){
    int i, j;
    SDL_Rect Rect_dest;
    SDL_Rect Rect_source;
    int posSprite;
    Rect_source.w = TAILLEBLOC;
    Rect_dest.w   = TAILLEBLOC;
    Rect_source.h = TAILLEBLOC;
    Rect_dest.h   = TAILLEBLOC;
    SDL_RenderCopy(renderer, salle->background, NULL, NULL);
    for(i = 0 ; i < salle->hauteur; i++)
    {
        for(j = 0 ; j < salle->largeur; j++)
        {
          posSprite = (salle->mat[i][j]-1);
          if(posSprite >= 0){
            Rect_dest.y = i * TAILLEBLOC;
            Rect_dest.x = j * TAILLEBLOC;
            Rect_source.x = posSprite * TAILLEBLOC;
            Rect_source.y = 0;
            SDL_RenderCopy(renderer, salle->tileset, &Rect_source, &Rect_dest);
            }
        }
    }
}


/**
 * \brief Fonction d'affichage du personnage
 *
 * @param renderer le pointeur vers le SDL_Renderer à utiliser
 * @param personnage la structure personnage à afficher
 * @param salle la salle où afficher le joueur
 */
void afficher_personnage(SDL_Renderer * renderer, personnage_t * personnage, salle_t * salle){
  SDL_Rect Rect_dest;
  SDL_RendererFlip flip=SDL_FLIP_NONE;
  if(personnage->direction != RIGHT)
    flip=SDL_FLIP_HORIZONTAL;
  Rect_dest.x = personnage->pos.x * TAILLEBLOC + personnage->delta.x;
  Rect_dest.y = personnage->pos.y * TAILLEBLOC + personnage->delta.y;
  Rect_dest.h = personnage->spriteActuel.h;
  Rect_dest.w = personnage->spriteActuel.w;
  SDL_RenderCopyEx(renderer, personnage->sprites, &(personnage->spriteActuel), &Rect_dest, 0, NULL, flip);
}

void affichage_complet(SDL_Renderer * renderer, salle_t * salle, personnage_t * personnage){
  SDL_Texture * textureSalle;
  textureSalle=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET, (salle->largeur)* TAILLEBLOC, (salle->hauteur) * TAILLEBLOC);
  SDL_SetRenderTarget(renderer, textureSalle);
  SDL_SetTextureBlendMode(textureSalle, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 0,0,0,0);
  SDL_RenderClear(renderer);
  afficher_salle(renderer,salle);
  afficher_personnage(renderer, personnage, salle);
  SDL_SetRenderTarget(renderer, NULL);
  SDL_RenderCopy(renderer, textureSalle, NULL, NULL);
  SDL_RenderPresent(renderer);
  SDL_DestroyTexture(textureSalle);
}

void miseAjourSprites(personnage_t * perso){
  if(perso->etat == IDLE){
    perso->spriteActuel.x=0;
    perso->spriteActuel.y=IDLE;
  }
  else if(perso->etat == FALLING){
    perso->spriteActuel.x=7*(perso->spriteActuel.w);
    perso->spriteActuel.y=JUMPING*(perso->spriteActuel.h);
  }
  if(perso->newEtat){
    if(perso->etat > IDLE && perso->etat < FALLING){
      perso->spriteActuel.x=0;
      perso->spriteActuel.y=perso->etat * (perso->spriteActuel.h);
    }
    if(perso->etat == ATTACKING)
      perso->spriteActuel.w = LARGEURSPRITEPERSATTACK;
    else
      perso->spriteActuel.w = LARGEURSPRITEPERS;
    perso->newEtat=FALSE;
    perso->evoSprite=0;
  }
  else{
    if(perso->etat > IDLE && perso->etat < FALLING){
      if(perso->evoSprite<=0){
        perso->spriteActuel.x+=perso->spriteActuel.w;
        if(perso->spriteActuel.x >= (perso->nbAnim[perso->etat])*perso->spriteActuel.w)
          perso->spriteActuel.x=0;
        perso->evoSprite = EVOSPRITES;
      }
      else (perso->evoSprite)--;
    }
  }

}
