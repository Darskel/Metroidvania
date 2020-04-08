#include "../headers/sdl_fonctions.h"

/**
 * \file sdl_fonctions.c
 * \brief Fichier qui regroupent les fonctions utilisées pour la gestion des graphismes et des évènements (SDL2)
 * \author Marie-Nina MUNAR L2 Info Le Mans
 * \version 4.2
 * \date 07/04/2020
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
  SDL_Surface * icon = NULL;

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
  printf("Frequence écran (en hz) : %i\n", mode->refresh_rate);
  res_h=mode->w - mode->w*OFFSETWINDOW;
  res_v=mode->h - mode->h*OFFSETWINDOW;

  *fenetre = SDL_CreateWindow(NOM_JEU, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, res_h, res_v, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : (SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE));
  if(!(*fenetre)){
    fprintf(stderr, "Erreur de creation de la fenetre : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  icon=IMG_Load("./sprites/autre/icone.png");
  SDL_SetWindowIcon(*fenetre,icon);

  *renderer = SDL_CreateRenderer(*fenetre, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
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
 * @param renderer le renderer utilisé
 * @param positionDepart la position de depart du joueur (en blocs)
 * @param positionDepartDelta la position de depart du joueur (en pixels à ajouter)
 * @return le pointeur sur la structure initialisée
 */
personnage_t * initialisation_personnage(SDL_Renderer * renderer, position_t positionDepart, position_t positionDepartDelta){
  personnage_t * personnage=malloc(sizeof(personnage_t));
  personnage->pv=1;
  personnage->pv_max=1;
  personnage->inv=0;
  personnage->direction=RIGHT;
  personnage->vit_dep=VITDEPPERS;
  personnage->vit_saut=VITSAUTPERS;
  personnage->vit_chute=VITCHUTEPERS;
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
  personnage->etat = IDLE;
  personnage->newEtat = FALSE;
  personnage->evoSprite = 0;
  personnage->nbPxSaut = 0;
  personnage->nbSaut=0;
  personnage->jpCd = 0;
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
 * \brief Fonction de destruction des types de monstres
 *
 */
void destroy_typeentites(void){
  /*for(int i=0; i<NBTYPEMONSTRE; i++){
    SDL_DestroyTexture((typesMonstre[i]).sprites);
    free((typesMonstre[i]).nbAnim);
  }Pour tout les types de monstres*/
  SDL_DestroyTexture((typesMonstre[-SERPENTBLEU -1]).sprites);
  free((typesMonstre[-SERPENTBLEU -1]).nbAnim);
  SDL_DestroyTexture((typesMonstre[-FLECHE -1]).sprites);
  free((typesMonstre[-FLECHE -1]).nbAnim);
}

/**
 * \brief Fonction d'initialisation des types monstres
 *
 * @param renderer le pointeur vers le renderer utilisé par les textures
 */
void initialiser_typeentites(SDL_Renderer * renderer){
  creerTypeEntite();
  /*for(int i=0; i<NBTYPEMONSTRE; i++){
    typesMonstre[i].sprites = initialiser_texture(typesMonstre[i].path, renderer);
  }*/
  typesMonstre[-SERPENTBLEU -1].sprites = initialiser_texture(typesMonstre[-SERPENTBLEU -1].path, renderer);
  typesMonstre[-FLECHE - 1].sprites = initialiser_texture(typesMonstre[-FLECHE -1].path, renderer);
}


/**
 * \brief Fonction d'initialisation de la salle
 *
 * @param renderer pointeur vers le renderer utilisé
 * @param nomFichier une chaine de caracteres qui contient le nom du fichier de la salle
 * @param tileset le pointeur vers la texture de tileset
 * @return un pointeur sur la structure salle initialisée
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
 * \brief Fonction de destruction de la structure salle
 *
 * @param salle pointeur sur le pointeur de la structure salle à détruire
 */
void destroy_salle(salle_t ** salle){
  SDL_DestroyTexture((*salle)->background);
  nettoyerSalle(salle);
  *salle=NULL;
}

/**
 * \brief Fonction d'affichage de la salle (fait en coopération avec Yannis Allain)
 *
 * @param renderer le pointeur vers le SDL_Renderer à utiliser
 * @param salle le pointeur sur la structure salle à afficher
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
          if(posSprite >= NBBLOCSTILESET)
            posSprite=NBBLOCSTILESET -1;
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
 * @param personnage le pointeur sur la structure personnage à afficher
 * @param salle le pointeur sur la salle où afficher le joueur
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

/**
 * \brief Fonction d'affichage des entités
 *
 * @param renderer le pointeur vers le SDL_Renderer à utiliser
 * @param salle le pointeur sur la salle où afficher l'entité
 */
void afficher_entites(SDL_Renderer * renderer, salle_t * salle){
  monstre_t entite;
  SDL_Rect Rect_dest;
  SDL_Rect Rect_source;
  SDL_RendererFlip flip=SDL_FLIP_NONE;
  enTete(salle->listeEntite);
  while(!horsListe(salle->listeEntite)){
    flip=SDL_FLIP_NONE;
    valeurElm(salle->listeEntite, &entite);
    if(entite.direction != RIGHT)
      flip=SDL_FLIP_HORIZONTAL;
    Rect_source.x = entite.spriteActuel.x;
    Rect_source.y = IDLE;
    Rect_source.h = entite.spriteActuel.h;
    Rect_source.w = entite.spriteActuel.w;
    Rect_dest.x = entite.pos.x * TAILLEBLOC + entite.delta.x;
    Rect_dest.y = entite.pos.y * TAILLEBLOC + entite.delta.y;
    Rect_dest.h = entite.spriteActuel.h;
    Rect_dest.w = entite.spriteActuel.w;
    SDL_RenderCopyEx(renderer, entite.type->sprites, &Rect_source, &Rect_dest, 0, NULL, flip);
    suivant(salle->listeEntite);
  }
}

/**
 * \brief Fonction d'affichage complet (salle et personnage)
 *
 * @param renderer le pointeur vers le SDL_Renderer à utiliser
 * @param salle le pointeur sur la salle où afficher le joueur et les entités
 * @param personnage le pointeur sur la structure personnage à afficher
 */
void affichage_complet(SDL_Renderer * renderer, salle_t * salle, personnage_t * personnage){
  SDL_Texture * textureSalle;
  SDL_Rect Rect_dest;
  int maxw;
  int maxh;
  float ratioFenetre;
  float ratioSalle;

  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
  SDL_RenderClear(renderer);

  SDL_GetRendererOutputSize(renderer, &maxw, &maxh);

  ratioFenetre = (maxw * 1.0) / (maxh * 1.0);
  ratioSalle = (salle->largeur * 1.0) / (salle->hauteur * 1.0);

  if(ratioFenetre<ratioSalle){
    Rect_dest.w = maxw;
    Rect_dest.h = Rect_dest.w /ratioSalle;
  }
  else{
    Rect_dest.h = maxh;
    Rect_dest.w = Rect_dest.h * ratioSalle;
  }

  Rect_dest.x = (maxw - Rect_dest.w)/2 ;
  Rect_dest.y = (maxh - Rect_dest.h)/2 ;


  textureSalle=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET, (salle->largeur)* TAILLEBLOC, (salle->hauteur) * TAILLEBLOC);
  SDL_SetRenderTarget(renderer, textureSalle);
  SDL_SetTextureBlendMode(textureSalle, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
  SDL_RenderClear(renderer);
  afficher_salle(renderer,salle);
  afficher_entites(renderer, salle);
  afficher_personnage(renderer, personnage, salle);
  SDL_SetRenderTarget(renderer, NULL);
  SDL_RenderCopy(renderer, textureSalle, NULL, &Rect_dest);
  SDL_RenderPresent(renderer);
  SDL_DestroyTexture(textureSalle);
}

/**
 * \brief Fonction qui fait évoluer les sprites du personnage (animation)
 *
 * @param perso le pointeur sur la structure personnage à faire évoluer
 */
void miseAjourSprites(personnage_t * perso){
  if(perso->etat == IDLE){
    perso->spriteActuel.x=IDLE;
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

/**
 * \brief Fonction qui fait évoluer les sprites de l'entite (animation)
 *
 * @param salle le pointeur vers la salle où trouver la liste d'entités avec les sprites à mettre à jour
 */
void miseAjourSpritesEntites(salle_t * salle){
  /*monstre_t entite;

  enTete(salle->listeEntite);
  while(!horsListe(salle->listeEntite)){
    valeurElm(salle->listeEntite, &entite);

    Rect_source.x = entite.spriteActuel.x;
    Rect_source.y = IDLE;
    Rect_source.h = entite.spriteActuel.h;
    Rect_source.w = entite.spriteActuel.w;
    Rect_dest.x = entite.pos.x * TAILLEBLOC + entite.delta.x;
    Rect_dest.y = entite.pos.y * TAILLEBLOC + entite.delta.y;
    Rect_dest.h = entite.spriteActuel.h;
    Rect_dest.w = entite.spriteActuel.w;
    suivant(salle->listeEntite);
  }

  if(perso->etat == IDLE){
    perso->spriteActuel.x=IDLE;
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
  }*/

}


/**
 * \brief Fonction d'affichage d'un écran noir d'une durée choisie
 *
 * @param renderer le pointeur vers le SDL_Renderer à utiliser
 * @param ms un entier correspondant à la durée d'affichage de l'écran noir (en millisecondes)
 */
void ecranNoir(SDL_Renderer * renderer, int ms){
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(ms);
}

/**
 * \brief Fonction qui détecte si le Konami code a été rentré, dans la bonne salle, et qui créé l'easter egg en conséquence (modifie la matrice de la salle)
 *
 * @param perso le pointeur vers la structure personnage (utile en cas de collision avec les blocs créés)
 * @param salle le pointeur vers la structure salle qui sera modifié en cas de Konami code
 * @param konami le tableau de caractère correspondant au code en cours (inputs)
 * @param indKon le pointeur vers l'entier qui correspond au nombre de caractères actuellement dans le code entré
 * @param kon le pointeur vers un booléen qui vaut FALSE si le Konami code n'a pas été entré et TRUE si c'est le cas
 */
void konamicode(personnage_t * perso, salle_t * salle, char * konami, int * indKon, boolean_t * kon){
  char verifCode[TAILLEKONAMI] = KONAMICODE;
  int indVerif;
  //tester konamicode
  if(*indKon){
    if(!strcmp(salle->nomFichier,"salle_entree_grotte.txt") && !(*kon)){
      for(indVerif = 0; (indVerif < *indKon) && (konami[indVerif] == verifCode[indVerif]); indVerif++);
      if(*indKon != indVerif){
        *indKon = 0;
        for(int i = 0; i < TAILLEKONAMI; i++)
          konami[i] = '\0';
      }else{
        if(*indKon == TAILLEKONAMI){
          //effectuer code
          printf("Konami code effectué !\n");
          salle->mat[9][9] = 34;
          salle->mat[10][9] = 1;
          salle->mat[10][8] = 34;
          salle->mat[10][10] = 34;
          if(!persValidDep(perso,salle)){
            perso->pos.x = 5;
            perso->delta.x = TAILLEBLOC - 1 + OFFSETHITBOX;
            if(perso->delta.x >= TAILLEBLOC)
              perso->delta.x %= TAILLEBLOC;
          }
          *kon = TRUE;
          for(int i = 0; i < TAILLEKONAMI; i++)
            konami[i] = '\0';
        }
      }
    }else
      *indKon = 0;
  }
}
