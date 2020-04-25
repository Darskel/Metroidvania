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
  res_h=mode->w;
  res_v=mode->h;

  *fenetre = SDL_CreateWindow(NOM_JEU, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, res_h, res_v, fullscreen ? SDL_WINDOW_FULLSCREEN : (SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED));
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

  SDL_FreeSurface(icon);

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
SDL_Texture * initialiser_texture(char * path, SDL_Renderer * renderer, boolean_t estTarget){
  SDL_Surface *surface = NULL;
  SDL_Texture *tmp = NULL;
  SDL_Texture *texture =NULL;
  Uint32 pixelFormatEnumTmp;

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

  if(estTarget){
    SDL_QueryTexture(tmp,&pixelFormatEnumTmp,NULL,NULL,NULL);
    const char* TexturePixelFormatName = SDL_GetPixelFormatName(pixelFormatEnumTmp);
    texture = SDL_CreateTexture(renderer, pixelFormatEnumTmp,
                                SDL_TEXTUREACCESS_TARGET, surface->w, surface->h);
    if(texture==NULL){
        printf("Surface pixel format : %s --- %s\n", TexturePixelFormatName, path);
        fprintf(stderr, "Erreur SDL_CreateTexture : %s", SDL_GetError());
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
  else{
    SDL_FreeSurface(surface);
    return tmp;
  }
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
  personnage->pv_max=PVMAX;
  personnage->pv=personnage->pv_max;
  personnage->kb=0;
  personnage->inv=0;
  personnage->clign=FREQCLIGN;
  personnage->direction=RIGHT;
  personnage->vit_dep=VITDEPPERS;
  personnage->vit_saut=VITSAUTPERS;
  personnage->vit_chute=VITCHUTEPERS;
  personnage->vit_att=VITATTACKPERS;
  personnage->pos=positionDepart;
  personnage->delta=positionDepartDelta;
  personnage->sprites=initialiser_texture(PLAYERSPRITESPATH, renderer, FALSE);
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
  personnage->inventaireTileset = initialiser_texture(INVENTAIREPATH, renderer, FALSE);
  personnage->nomObj[0] = "cle bleue";
  personnage->nomObj[1] = "cle rouge";
  personnage->nomObj[2] = "cle rouillee";
  personnage->nomObj[3] = "cle verte";
  personnage->nomObj[4] = "discoshroom";
  personnage->nomObj[5] = "double saut";
  personnage->nomObj[6] = "huile";
  personnage->nomObj[7] = "renard";
  return personnage;
}

void afficherInventaire(SDL_Renderer * renderer, personnage_t * personnage, SDL_Texture * textureSalle){
  SDL_Rect boite;
  SDL_Rect objPlacement;
  SDL_Rect objTileset;
  int maxw;
  int maxh;
  SDL_SetRenderTarget(renderer, textureSalle);
  SDL_GetRendererOutputSize(renderer, &maxw, &maxh);
  objPlacement.w = INVENTAIRESIZE;
  objPlacement.h = INVENTAIRESIZE;
  objTileset.w = INVENTAIRESIZE;
  objTileset.h = INVENTAIRESIZE;
  boite.w = INVENTAIRESIZE * (TAILLE_INVENTAIRE+2);
  boite.h = INVENTAIRESIZE + 2;
  boite.x = maxw - maxw/100 - boite.w;
  boite.y = maxh/200;
  objPlacement.x= boite.x + 2;
  objPlacement.y= boite.y + 1;
  objTileset.x=0;
  objTileset.y=0;
  SDL_SetRenderDrawColor(renderer, 10, 10, 10, 200);
  SDL_RenderFillRect(renderer, &boite);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_SetRenderTarget(renderer, NULL);
  for(int i=0; i<TAILLE_INVENTAIRE; i++){
    if(personnage->inventaire[i]){
      SDL_SetRenderTarget(renderer, textureSalle);
      SDL_RenderCopy(renderer, personnage->inventaireTileset, &objTileset, &objPlacement);
      SDL_SetRenderTarget(renderer, NULL);
      objPlacement.x += INVENTAIRESIZE + 2;
    }
    objTileset.x += INVENTAIRESIZE;
  }
}


/**
 * \brief Fonction de destruction de la structure personnage
 *
 * @param personnage pointeur sur le pointeur de la structure à détruire
 */
void destroy_personnage(personnage_t ** personnage){
  SDL_DestroyTexture((*personnage)->sprites);
  SDL_DestroyTexture((*personnage)->inventaireTileset);
  free((*personnage)->nbAnim);
  free(*personnage);
  *personnage=NULL;
}

/**
 * \brief Fonction de destruction des types de monstres
 *
 */
void destroy_typeentites(void){
  for(int i=0; i<NBTYPEMONSTRE; i++){
    SDL_DestroyTexture((typesMonstre[i]).sprites);
    free((typesMonstre[i]).nbAnim);
  }//Pour tout les types de monstres*/
  /*SDL_DestroyTexture((typesMonstre[-SERPENTBLEU -1]).sprites);
  free((typesMonstre[-SERPENTBLEU -1]).nbAnim);
  SDL_DestroyTexture((typesMonstre[-COEUR -1]).sprites);
  free((typesMonstre[-COEUR -1]).nbAnim);
  SDL_DestroyTexture((typesMonstre[-FLECHE -1]).sprites);
  free((typesMonstre[-FLECHE -1]).nbAnim);*/
}

/**
 * \brief Fonction d'initialisation des types monstres
 *
 * @param renderer le pointeur vers le renderer utilisé par les textures
 */
void initialiser_typeentites(SDL_Renderer * renderer){
  creerTypeEntite();
  for(int i=0; i<NBTYPEMONSTRE; i++){
    typesMonstre[i].sprites = initialiser_texture(typesMonstre[i].path, renderer, FALSE);
  }//*/
  /*typesMonstre[-SERPENTBLEU -1].sprites = initialiser_texture(typesMonstre[-SERPENTBLEU -1].path, renderer);
  typesMonstre[-COEUR - 1].sprites = initialiser_texture(typesMonstre[-COEUR -1].path, renderer);
  typesMonstre[-FLECHE - 1].sprites = initialiser_texture(typesMonstre[-FLECHE -1].path, renderer);*/
}


/**
 * \brief Fonction d'initialisation de la salle
 *
 * @param renderer pointeur vers le renderer utilisé
 * @param nomFichier une chaine de caracteres qui contient le nom du fichier de la salle
 * @param tileset le pointeur vers la texture de tileset
 * @return un pointeur sur la structure salle initialisée
 */
salle_t * initialiser_salle(SDL_Renderer * renderer, char* nomFichier, personnage_t* perso){
  salle_t * salle=NULL;
  char nom_bg[100];

  strcpy(nom_bg,DIRBG);
  strcat(nom_bg, nomFichier);
  nom_bg[strlen(nom_bg) - 3] = '\0';
  strcat(nom_bg, "png");

  lireSalle(nomFichier, &salle, perso);
  salle->background=initialiser_texture(nom_bg, renderer, FALSE);
  salle->tileset=initialiser_texture(TILESETPATH, renderer, FALSE);
  return salle;
}

/**
 * \brief Fonction de destruction de la structure salle
 *
 * @param salle pointeur sur le pointeur de la structure salle à détruire
 */
void destroy_salle(salle_t ** salle){
  SDL_DestroyTexture((*salle)->background);
  SDL_DestroyTexture((*salle)->tileset);
  nettoyerSalle(salle);
  *salle=NULL;
}

/**
 * \brief Fonction d'affichage de la salle (fait en coopération avec Yannis Allain)
 *
 * @param renderer le pointeur vers le SDL_Renderer à utiliser
 * @param salle le pointeur sur la structure salle à afficher
 */
void afficher_salle(SDL_Renderer * renderer, salle_t * salle, SDL_Texture * textureSalle){
    int i, j;
    SDL_Rect Rect_dest;
    SDL_Rect Rect_source;
    int posSprite;
    Rect_source.w = TAILLEBLOC;
    Rect_dest.w   = TAILLEBLOC;
    Rect_source.h = TAILLEBLOC;
    Rect_dest.h   = TAILLEBLOC;
    SDL_SetRenderTarget(renderer, textureSalle);
    SDL_RenderCopy(renderer, salle->background, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);
    for(i = 0 ; i < salle->hauteur; i++)
    {
        for(j = 0 ; j < salle->largeur; j++)
        {
          posSprite = (salle->mat[i][j]-1);
          if(posSprite >= NBBLOCSTILESET)
            posSprite=NBLOCERREUR-1;
          if(posSprite >= 0){
            Rect_dest.y = i * TAILLEBLOC;
            Rect_dest.x = j * TAILLEBLOC;
            Rect_source.x = posSprite * TAILLEBLOC;
            Rect_source.y = 0;
            SDL_SetRenderTarget(renderer, textureSalle);
            SDL_RenderCopy(renderer, salle->tileset, &Rect_source, &Rect_dest);
            SDL_SetRenderTarget(renderer, NULL);
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
void afficher_personnage(SDL_Renderer * renderer, personnage_t * personnage, salle_t * salle, SDL_Texture * textureSalle){
  SDL_Rect Rect_dest;
  SDL_RendererFlip flip=SDL_FLIP_NONE;
  if(personnage->direction != RIGHT)
    flip=SDL_FLIP_HORIZONTAL;
  Rect_dest.x = personnage->pos.x * TAILLEBLOC + personnage->delta.x;
  Rect_dest.y = personnage->pos.y * TAILLEBLOC + personnage->delta.y;
  Rect_dest.h = personnage->spriteActuel.h;
  Rect_dest.w = personnage->spriteActuel.w;
  SDL_SetRenderTarget(renderer, textureSalle);
  SDL_RenderCopyEx(renderer, personnage->sprites, &(personnage->spriteActuel), &Rect_dest, 0, NULL, flip);
  SDL_SetRenderTarget(renderer, NULL);
}

/**
 * \brief Fonction d'affichage des entités
 *
 * @param renderer le pointeur vers le SDL_Renderer à utiliser
 * @param salle le pointeur sur la salle où afficher l'entité
 */
void afficher_entites(SDL_Renderer * renderer, salle_t * salle, SDL_Texture * textureSalle){
  monstre_t entite;
  SDL_Rect Rect_dest;
  SDL_RendererFlip flip=SDL_FLIP_NONE;
  enTete(salle->listeEntite);
  while(!horsListe(salle->listeEntite)){
    flip=SDL_FLIP_NONE;
    valeurElm(salle->listeEntite, &entite);
    if(entite.direction != RIGHT)
      flip=SDL_FLIP_HORIZONTAL;
    Rect_dest.x = entite.pos.x * TAILLEBLOC + entite.delta.x;
    Rect_dest.y = entite.pos.y * TAILLEBLOC + entite.delta.y;
    Rect_dest.h = entite.spriteActuel.h;
    Rect_dest.w = entite.spriteActuel.w;
    SDL_SetRenderTarget(renderer, textureSalle);
    if(SDL_RenderCopyEx(renderer, entite.type->sprites, &(entite.spriteActuel), &Rect_dest, 0, NULL, flip));
    SDL_SetRenderTarget(renderer, NULL);
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
void affichage_complet(SDL_Renderer * renderer, salle_t * salle, personnage_t * personnage, int * inventaireAffiche){
  SDL_Texture * textureSalle;
  SDL_Rect Rect_dest;
  int maxw;
  int maxh;
  float ratioFenetre;
  float ratioSalle;
  SDL_Texture * coeurImage;
  coeurImage = initialiser_texture("./sprites/entite/coeur/coeur-small.png", renderer, FALSE);

  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
  //SDL_RenderClear(renderer);
  SDL_RenderFillRect(renderer, NULL);

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


  textureSalle=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_TARGET, (salle->largeur)* TAILLEBLOC, (salle->hauteur) * TAILLEBLOC);
  SDL_SetRenderTarget(renderer, textureSalle);
  SDL_SetTextureBlendMode(textureSalle, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
  //SDL_RenderClear(renderer);
  SDL_RenderFillRect(renderer, NULL);
  SDL_SetRenderTarget(renderer, NULL);
  afficher_salle(renderer,salle, textureSalle);
  afficher_entites(renderer,salle, textureSalle);
  if(personnage->clign%FREQCLIGN == 0 || (personnage->kb == FALSE && personnage->inv<INVUDELAY))
    afficher_personnage(renderer, personnage, salle, textureSalle);
  afficherVieCoeurs(renderer, personnage, coeurImage, textureSalle);
  //afficherVieJauge(renderer, personnage);
  if(*inventaireAffiche > 0){
    afficherInventaire(renderer, personnage, textureSalle);
    if(*inventaireAffiche < INVENTAIRETIME)
      (*inventaireAffiche)--;
  }
  SDL_SetRenderTarget(renderer, NULL);
  SDL_RenderCopy(renderer, textureSalle, NULL, &Rect_dest);
  SDL_RenderPresent(renderer);
  SDL_DestroyTexture(textureSalle);
  SDL_DestroyTexture(coeurImage);
}

/**
 * \brief Fonction qui fait évoluer les sprites du personnage (animation)
 *
 * @param perso le pointeur sur la structure personnage à faire évoluer
 */
void miseAjourSprites(personnage_t * perso){
  if((perso->inv || perso->kb)){ //&& perso->hit ){
    perso->clign--;
    if(perso->clign<0)
      perso->clign=FREQCLIGN;
  }
  else
    perso->clign=FREQCLIGN;
  if(perso->etat == IDLE){
    perso->spriteActuel.x=IDLE;
    perso->spriteActuel.y=IDLE;
  }
  else if(perso->etat == FALLING){
    perso->spriteActuel.x=(perso->nbAnim[JUMPING] - 1)*(perso->spriteActuel.w);
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
    //perso->evoSprite=0;
  }
  else{
    if(perso->etat > IDLE && perso->etat < FALLING){
      if(perso->evoSprite<=0){
        perso->spriteActuel.x+=perso->spriteActuel.w;
        if(perso->etat == JUMPING){
          if(perso->spriteActuel.x >= (perso->nbAnim[perso->etat])*perso->spriteActuel.w)
            perso->spriteActuel.x=(perso->nbAnim[perso->etat]-1)*perso->spriteActuel.w;
          if(perso->spriteActuel.x <0)
            perso->spriteActuel.x=0;
        }
        else{
          if(perso->spriteActuel.x >= (perso->nbAnim[perso->etat])*perso->spriteActuel.w)
            perso->spriteActuel.x=0;
        }
        if(perso->etat == ATTACKING)
          perso->evoSprite = EVOSPRITESATTACK;
        else
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
  monstre_t * entite = NULL;

  enTete(salle->listeEntite);
  while(!horsListe(salle->listeEntite)){
    entite=malloc(sizeof(monstre_t));
    valeurElm(salle->listeEntite, entite);

    if(entite->etat == IDLE){
      entite->spriteActuel.x=IDLE;
      entite->spriteActuel.y=IDLE;
    }

    /*else if(entite->etat == FALLING){
      entite->spriteActuel.x=(entite->type->nbAnim[JUMPING] -1)*(entite->spriteActuel.w);
      entite->spriteActuel.y=JUMPING*(entite->spriteActuel.h);
    }*/
    if(entite->newEtat){
      if(entite->etat > IDLE && entite->etat <= FALLING){
        entite->spriteActuel.x=0;
        entite->spriteActuel.y=entite->etat * (entite->spriteActuel.h);
      }
      entite->newEtat=FALSE;
      //entite->evoSprite=0;
    }
    else{
      if(entite->etat > IDLE && entite->etat <= FALLING){
        entite->spriteActuel.y=entite->etat * (entite->spriteActuel.h);
        if(entite->evoSprite<=0){
          entite->spriteActuel.x+=entite->spriteActuel.w;
          if(entite->etat == JUMPING || entite->etat==FALLING){
            if(entite->spriteActuel.x >= (entite->type->nbAnim[entite->etat])*entite->spriteActuel.w)
              entite->spriteActuel.x=(entite->type->nbAnim[entite->etat]-1)*entite->spriteActuel.w;
            if(entite->spriteActuel.x <0)
              entite->spriteActuel.x=0;
          }
          else{
          if(entite->spriteActuel.x >= (entite->type->nbAnim[entite->etat])*(entite->spriteActuel.w))
            entite->spriteActuel.x=0;
          }
          entite->evoSprite = entite->type->vitesseAnim;
        }
        else (entite->evoSprite)--;
      }
    }
    //printf("%d_%d\n",entite->etat,entite->spriteActuel.x);

    modifElm(salle->listeEntite, entite);
    free(entite);
    entite=NULL;
    suivant(salle->listeEntite);
  }
}


/**
 * \brief Fonction d'affichage d'un écran noir d'une durée choisie
 *
 * @param renderer le pointeur vers le SDL_Renderer à utiliser
 * @param ms un entier correspondant à la durée d'affichage de l'écran noir (en millisecondes)
 */
void ecranNoir(SDL_Renderer * renderer, int ms){
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderFillRect(renderer,NULL);
    //SDL_RenderClear(renderer);
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

void jeu(SDL_Window * fenetre, SDL_Renderer ** renderer, SDL_DisplayMode mode, SDL_Joystick * pJoystick, int fullscreen){
  SDL_Event event;
  Uint32 frameStart;
  int frameTime;
  int mousex;
  int mousey;
  int messageRes;
  Sint16 x_move;
  Sint16 y_move;
  int inventaireAffiche=0;
  position_t positionDepart;
  position_t positionDepartDelta;
  //boolean_t windowtouched=FALSE;
  //SDL_Texture * tileset=NULL;
  salle_t * salle=NULL;
  personnage_t * perso=NULL;
  char * salle_nom = NULL;
  boolean_t Gauche = FALSE;
  boolean_t Droite = FALSE;
  boolean_t tryJump = FALSE;
  boolean_t tryAtk = FALSE;
  boolean_t fin=FALSE;
  boolean_t salleChangee=FALSE;
  boolean_t kon = FALSE;
  char konami[TAILLEKONAMI];
  int indKon = 0;
  for(int i = 0; i < TAILLEKONAMI; i++)
    konami[i] = '\0';

  SDL_MessageBoxButtonData * buttons = NULL;

  SDL_SetRenderDrawColor(*renderer,0,0,0,255);
  //SDL_RenderClear(*renderer);
  SDL_RenderFillRect(*renderer, NULL);
  SDL_RenderPresent(*renderer);

  initialiser_typeentites(*renderer);
  //tileset=initialiser_texture(TILESETPATH, *renderer);

  salle=initialiser_salle(*renderer, NIVEAUTXT, perso);
  positionDepart.x = 1;
  positionDepartDelta.x = 0;
  positionDepart.y = salle->hauteur - HAUTEURHITBOXPERS/TAILLEBLOC -2;
  positionDepartDelta.y = TAILLEBLOC-1;
  ecranNoir(*renderer, 100);
  perso=initialisation_personnage(*renderer, positionDepart, positionDepartDelta);

  while(fin==FALSE){
    frameStart = SDL_GetTicks();
    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_QUIT: //Appui sur la croix quitte le programme (avec fenetre de dialogue pour confirmation)
          //Avec message box :
          buttons = malloc(2*sizeof(SDL_MessageBoxButtonData));
          buttons[0].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
          buttons[0].buttonid = 0;
          buttons[0].text = "Non";
          buttons[1].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
          buttons[1].buttonid = 1;
          buttons[1].text = "Oui";
          messageRes=afficherMessageBox(fenetre, buttons, 2, "Quitter ?", "Voulez-vous quitter ?", fullscreen);
          if(messageRes == 1)
            fin=TRUE;
          free(buttons);
          buttons=NULL;
          //Avec menu :
          /*
          fin=menuConfirmation(*renderer);
          */
          break;
        case SDL_WINDOWEVENT:
          //windowtouched=TRUE;
          destroy_typeentites();
          initialiser_typeentites(*renderer);
          SDL_DestroyTexture(perso->sprites);
          perso->sprites=initialiser_texture(PLAYERSPRITESPATH, *renderer, FALSE);
          //Si la salle devient mouvante (ou autre chose) (blocs, arrière plan), un destroy texture des textures associées est envisageable de la meme façon que pour le personnage
          break;
        case SDL_KEYUP:
          switch(event.key.keysym.sym){
            case SDLK_ESCAPE://Appui sur Echap quitte le programme
              //Avec message box :
              buttons = malloc(2*sizeof(SDL_MessageBoxButtonData));
              buttons[0].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
              buttons[0].buttonid = 0;
              buttons[0].text = "Non";
              buttons[1].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
              buttons[1].buttonid = 1;
              buttons[1].text = "Oui";
              messageRes=afficherMessageBox(fenetre, buttons, 2, "Quitter ?", "Voulez-vous quitter ?", fullscreen);
              if(messageRes == 1)
                fin=TRUE;
              free(buttons);
              buttons=NULL;
              //Avec menu :
              /*
              fin=menuConfirmation(*renderer);
              */
              break;
            case SDLK_LEFT:
            case SDLK_q:
              konami[indKon++] = 'l';
              Gauche=FALSE;
              break;
            case SDLK_RIGHT:
            case SDLK_d:
              konami[indKon++] = 'r';
              Droite=FALSE;
              break;
            case SDLK_UP:
            case SDLK_z:
            case SDLK_SPACE:
              konami[indKon++] = 'u';
              break;
            case SDLK_DOWN:
            case SDLK_s:
              konami[indKon++] = 'd';
              break;
            case SDLK_a:
              konami[indKon++] = 'a';
            case SDLK_i:
              if(inventaireAffiche==INVENTAIRETIME)
                inventaireAffiche--;
              break;
            case SDLK_b:
              konami[indKon++] = 'b';
              break;
            case SDLK_RETURN:
              konami[indKon++] = 's';
              break;
          }
          break;
        case SDL_KEYDOWN:
          switch(event.key.keysym.sym){
            case SDLK_LEFT:
            case SDLK_q:
              Gauche=TRUE;
              break;
            case SDLK_RIGHT:
            case SDLK_d:
              Droite=TRUE;
              break;
            case SDLK_UP:
            case SDLK_z:
            case SDLK_SPACE:
            //Cette façon de faire produit la possibilité de bunny hop (saut juste après un saut, quand le bouton reste appuyé)
              tryJump=TRUE;
                break;
            case SDLK_DOWN:
            case SDLK_s:
              break;
            case SDLK_e:
              tryAtk=TRUE;
              break;
            case SDLK_i:
            case SDLK_a:
              inventaireAffiche=INVENTAIRETIME;
              break;
          }
          break;
        case SDL_MOUSEMOTION :
          mousex=event.motion.x;
          mousey=event.motion.y;
          break;
        case SDL_JOYBUTTONDOWN :
        switch(event.jbutton.button){
            case 0 : //bouton A manette XBOX
              konami[indKon++] = 'a';
              tryJump=TRUE;
              break;
            case 1 : //bouton B manette XBOX
              konami[indKon++] = 'b';
              tryAtk=TRUE;
              break;
            case 3 : //bouton Y manette XBOX
              inventaireAffiche=INVENTAIRETIME;
              break;
            case 7 : //bouton Start manette XBOX
              konami[indKon++] = 's';
              break;
          }
        break;
        case SDL_JOYBUTTONUP :
          switch(event.jbutton.button){
            case 3 : //bouton Y manette XBOX
              if(inventaireAffiche==INVENTAIRETIME)
                inventaireAffiche--;
              break;
          }
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
        /*case SDL_JOYBALLMOTION :
          break;*/
        case SDL_JOYHATMOTION :
          switch(event.jhat.value){
            case SDL_HAT_CENTERED:
              Gauche = FALSE;
              Droite = FALSE;
              break;
            case SDL_HAT_LEFT:
              konami[indKon++] = 'l';
              Gauche = TRUE;
              break;
            case SDL_HAT_RIGHT:
              konami[indKon++] = 'r';
              Droite = TRUE;
              break;
            case SDL_HAT_UP:
              konami[indKon++] = 'u';
              tryJump=TRUE;
              break;
            case SDL_HAT_DOWN:
              konami[indKon++] = 'd';
              break;
          }
        }
      }
      if ( pJoystick != NULL ){
        x_move = SDL_JoystickGetAxis(pJoystick, 0);
        y_move = SDL_JoystickGetAxis(pJoystick, 1);
      }

      //if(windowtouched)


      konamicode(perso,salle,konami,&indKon,&kon);

      salle_nom=prendPorte(perso, (salle)->listePorte);
      if(salle_nom != NULL){
        destroy_salle(&salle);
        salle=initialiser_salle(*renderer, salle_nom, perso);
        salleChangee=TRUE;
        kon = FALSE;
        free(salle_nom);
        salle_nom=NULL;
      }

      depVert(perso, salle, tryJump);

      if(Gauche && !(perso)->kb){ // ajout de && !(*perso)->kb par Thomas: evite de changer la direction du regard pendant un knockback
        depGauche(perso, salle);
        if(!Droite)
          (perso)->direction = LEFT;
      }

      if(Droite && !(perso)->kb){ // ajout de && !(*perso)->kb par Thomas: evite de changer la direction du regard pendant un knockback
        depDroite(perso, salle);
        if(!Gauche)
          (perso)->direction = RIGHT;
      }

      if((!Gauche&&!Droite) || (Gauche&&Droite))
        if((perso)->etat == RUNNING)//ajouter par Thomas: on souhaite passer de RUNNING à IDLE mais pas de JUMPING à IDLE ou bien de ATTACKING à IDLE
          (perso)->etat=IDLE;

      tryJump=FALSE;

      attaquer(perso,salle,tryAtk);

      tryAtk=FALSE;

      evolution(perso,salle);

      miseAjourSprites(perso);
      miseAjourSpritesEntites(salle);

      if(salleChangee){
        ecranNoir(*renderer,150);
        salleChangee=FALSE;
      }

      SDL_SetRenderDrawColor(*renderer,0,0,0,255);
      //SDL_RenderClear(*renderer);
      SDL_RenderFillRect(*renderer, NULL);
      affichage_complet(*renderer, salle, perso, &inventaireAffiche);

      frameTime = SDL_GetTicks() - frameStart;
      if(frameTime < FRAMEDELAY){
        SDL_Delay(FRAMEDELAY - frameTime);
      }
    }
    destroy_salle(&salle);
    destroy_personnage(&perso);
    destroy_typeentites();
    //SDL_DestroyTexture(tileset);
}

void afficher_menu(SDL_Renderer * renderer, SDL_Texture * fond){
  SDL_Texture * texture;
  SDL_Rect Rect_dest;
  int maxw;
  int maxh;
  float ratioFenetre;
  float ratioMenu;

  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
  //SDL_RenderClear(renderer);
  SDL_RenderFillRect(renderer,NULL);

  SDL_GetRendererOutputSize(renderer, &maxw, &maxh);

  ratioFenetre = (maxw * 1.0) / (maxh * 1.0);
  ratioMenu = (TAILLEBGMENUW * 1.0) / (TAILLEBGMENUH * 1.0);

  if(ratioFenetre<ratioMenu){
    Rect_dest.w = maxw;
    Rect_dest.h = Rect_dest.w /ratioMenu;
  }
  else{
    Rect_dest.h = maxh;
    Rect_dest.w = Rect_dest.h * ratioMenu;
  }

  Rect_dest.x = (maxw - Rect_dest.w)/2 ;
  Rect_dest.y = (maxh - Rect_dest.h)/2 ;


  texture=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_TARGET, TAILLEBGMENUW, TAILLEBGMENUH);
  SDL_SetRenderTarget(renderer, texture);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
  SDL_RenderFillRect(renderer, NULL);
  //SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, fond, NULL, NULL);
  SDL_SetRenderTarget(renderer, NULL);
  SDL_RenderCopy(renderer, texture, NULL, &Rect_dest);
  SDL_RenderPresent(renderer);
  SDL_DestroyTexture(texture);
}

int afficherMessageBox(SDL_Window * fenetre, SDL_MessageBoxButtonData * buttons, int nbButtons, char * titre, char * message, int fullscreen){
  int buttonid;

  const SDL_MessageBoxColorScheme colorScheme = {
      { /* .colors (.r, .g, .b) */
          /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
          { 200,   200,   200 },
          /* [SDL_MESSAGEBOX_COLOR_TEXT] */
          {   0, 0,   0 },
          /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
          { 50, 50, 50 },
          /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
          {   255, 255, 255 },
          /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
          { 150, 150, 150 }
      }
  };
  const SDL_MessageBoxData messageboxdata = {
      SDL_MESSAGEBOX_INFORMATION, /* .flags */
      fenetre, /* .window */
      titre, /* .title */
      message, /* .message */
      nbButtons, /* .numbuttons */
      buttons, /* .buttons */
      &colorScheme /* .colorScheme */
  };
  if(fullscreen)
    SDL_SetWindowFullscreen(fenetre, 0);
  if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
      printf("error displaying message box\n");
      return -2;
  }
  if(fullscreen)
    SDL_SetWindowFullscreen(fenetre, SDL_WINDOW_FULLSCREEN);
  return buttonid;

}

menu_t * creerMenuDemarrage(SDL_Renderer * renderer){
  menu_t * menu = malloc(sizeof(menu_t));
  int nbBoutons = 4;
  int nbTextes = 1;
  menu->nbBoutons=nbBoutons;
  menu->nbTextes=nbTextes;
  menu->etiquette = "Menu Principal";
  //Fond :
  menu->fond = initialiser_texture("./sprites/menu/menu.png", renderer, FALSE);
  //Texte :
  menu_texte_t * tabTextes = malloc(nbTextes * sizeof(menu_texte_t));
  menu->tabTextes=tabTextes;
  tabTextes[0].id=0;
  tabTextes[0].etiquette="Diskosieni";
  tabTextes[0].parent=menu;
  tabTextes[0].texture = initialiser_texture("./sprites/menu/diskosieni.png", renderer, FALSE);

  //Boutons :
  menu_bouton_t * tabBoutons = malloc(nbBoutons * sizeof(menu_bouton_t));
  menu->tabBoutons=tabBoutons;
  tabBoutons[0].id=0;
  tabBoutons[0].etiquette="Commencer";
  tabBoutons[0].parent=menu;
  tabBoutons[0].texture = malloc(3 * sizeof(SDL_Texture *));
  tabBoutons[0].texture[RELAXED] = initialiser_texture("./sprites/bouton/commencer.png", renderer, FALSE);
  tabBoutons[0].texture[HIGHLIGHTED] = initialiser_texture("./sprites/bouton/commencer_selectionne.png", renderer, FALSE);
  tabBoutons[0].texture[PRESSED] = initialiser_texture("./sprites/bouton/commencer_clique.png", renderer, FALSE);

  tabBoutons[1].id=1;
  tabBoutons[1].etiquette="Continuer";
  tabBoutons[1].parent=menu;
  tabBoutons[1].texture = malloc(3 * sizeof(SDL_Texture *));
  tabBoutons[1].texture[RELAXED] = initialiser_texture("./sprites/bouton/continuer.png", renderer, FALSE);
  tabBoutons[1].texture[HIGHLIGHTED] = initialiser_texture("./sprites/bouton/continuer_selectionne.png", renderer, FALSE);
  tabBoutons[1].texture[PRESSED] = initialiser_texture("./sprites/bouton/continuer_clique.png", renderer, FALSE);

  tabBoutons[2].id=2;
  tabBoutons[2].etiquette="Options";
  tabBoutons[2].parent=menu;
  tabBoutons[2].texture = malloc(3 * sizeof(SDL_Texture *));
  tabBoutons[2].texture[RELAXED] = initialiser_texture("./sprites/bouton/options.png", renderer, FALSE);
  tabBoutons[2].texture[HIGHLIGHTED] = initialiser_texture("./sprites/bouton/options_selectionne.png", renderer, FALSE);
  tabBoutons[2].texture[PRESSED] = initialiser_texture("./sprites/bouton/options_clique.png", renderer, FALSE);

  tabBoutons[3].id=3;
  tabBoutons[3].etiquette="Quitter";
  tabBoutons[3].parent=menu;
  tabBoutons[3].texture = malloc(3 * sizeof(SDL_Texture *));
  tabBoutons[3].texture[RELAXED] = initialiser_texture("./sprites/bouton/quitter.png", renderer, FALSE);
  tabBoutons[3].texture[HIGHLIGHTED] = initialiser_texture("./sprites/bouton/quitter_selectionne.png", renderer, FALSE);
  tabBoutons[3].texture[PRESSED] = initialiser_texture("./sprites/bouton/quitter_clique.png", renderer, FALSE);

  //CERTAINEMENT PAS TERMINE

  return menu;
}

menu_t * creerMenuInGame(SDL_Renderer * renderer){
  menu_t * menu = malloc(sizeof(menu_t));
  //A FAIRE
  return menu;
}

menu_t * creerMenuConfirmation(SDL_Renderer * renderer){
  menu_t * menu = malloc(sizeof(menu_t));
  int nbBoutons = 2;
  int nbTextes = 1;
  menu->nbBoutons=nbBoutons;
  menu->nbTextes=nbTextes;
  menu->etiquette = "Menu Principal";
  //Fond :
  menu->fond = initialiser_texture("./sprites/menu/menu.png", renderer, FALSE);
  //Texte :
  menu_texte_t * tabTextes = malloc(nbTextes * sizeof(menu_texte_t));
  menu->tabTextes=tabTextes;
  tabTextes[0].id=0;
  tabTextes[0].etiquette="Voulez-vous quitter ?";
  tabTextes[0].parent=menu;
  tabTextes[0].texture = initialiser_texture("./sprites/menu/voulezvousquitter.png", renderer, FALSE);
  //Boutons :
  menu_bouton_t * tabBoutons = malloc(nbBoutons * sizeof(menu_bouton_t));
  menu->tabBoutons=tabBoutons;

  tabBoutons[0].id=0;
  tabBoutons[0].etiquette="Oui";
  tabBoutons[0].parent=menu;
  tabBoutons[0].texture = malloc(3 * sizeof(SDL_Texture *));
  tabBoutons[0].texture[RELAXED] = initialiser_texture("./sprites/bouton/oui.png", renderer, FALSE);
  tabBoutons[0].texture[HIGHLIGHTED] = initialiser_texture("./sprites/bouton/oui_selectionne.png", renderer, FALSE);
  tabBoutons[0].texture[PRESSED] = initialiser_texture("./sprites/bouton/oui_clique.png", renderer, FALSE);

  tabBoutons[1].id=1;
  tabBoutons[1].etiquette="Non";
  tabBoutons[1].parent=menu;
  tabBoutons[1].texture = malloc(3 * sizeof(SDL_Texture *));
  tabBoutons[1].texture[RELAXED] = initialiser_texture("./sprites/bouton/non.png", renderer, FALSE);
  tabBoutons[1].texture[HIGHLIGHTED] = initialiser_texture("./sprites/bouton/non_selectionne.png", renderer, FALSE);
  tabBoutons[1].texture[PRESSED] = initialiser_texture("./sprites/bouton/non_clique.png", renderer, FALSE);

  //CERTAINEMENT PAS TERMINE

  return menu;
}

void detruireMenu(menu_t ** menu){
  //A FAIRE
  free(*menu);
}

boolean_t menuConfirmation(SDL_Renderer * renderer){
  //A FAIRE
  return FALSE;
}

void afficherMenu(SDL_Renderer * renderer, menu_t * menu){
  //A FAIRE
}


void afficherVieJauge(SDL_Renderer * renderer, personnage_t * personnage, SDL_Texture * textureSalle){
  SDL_Rect exterieur;
  SDL_Rect interieur;
  int maxw;
  int maxh;
  SDL_SetRenderTarget(renderer, textureSalle);
  SDL_GetRendererOutputSize(renderer, &maxw, &maxh);
  interieur.h = maxh/50;
  exterieur.w = maxw/8 + 2;
  exterieur.h = interieur.h + 2;
  exterieur.x = maxw/100 + 1;
  exterieur.y = maxh/200 + 1 + 10;
  interieur.x = exterieur.x +1;
  interieur.y = exterieur.y +1;
  interieur.w = exterieur.w - exterieur.w/((personnage->pv_max*1.0)/((personnage->pv_max - personnage->pv)*1.0));
  if(interieur.w>=2)
    interieur.w -= 2;
  SDL_SetRenderDrawColor(renderer,115, 23, 45, 250);
  SDL_RenderFillRect(renderer, &exterieur);
  SDL_SetRenderDrawColor(renderer,20, 160, 46, 255);
  SDL_RenderFillRect(renderer, &interieur);
  SDL_SetRenderDrawColor(renderer,0,0,0,255);
  SDL_SetRenderTarget(renderer, NULL);
}

void afficherVieCoeurs(SDL_Renderer * renderer, personnage_t * personnage, SDL_Texture * coeurImage, SDL_Texture * textureSalle){
  SDL_Rect boite;
  SDL_Rect coeur;
  int maxw;
  int maxh;
  int coeurw;
  int coeurh;
  int pv=0;
  SDL_SetRenderTarget(renderer, textureSalle);
  SDL_GetRendererOutputSize(renderer, &maxw, &maxh);
  SDL_QueryTexture(coeurImage, NULL, NULL, &coeurw, &coeurh);
  coeur.w = coeurw;
  coeur.h = coeurh;
  boite.w = (coeur.w+2) *(personnage->pv_max);
  boite.h = coeur.h +2;
  boite.x = maxw/100;
  boite.y = maxh/200;
  coeur.x = boite.x +1;
  coeur.y = boite.y +1;
  SDL_SetRenderDrawColor(renderer, 10, 10, 10, 100);
  SDL_RenderFillRect(renderer, &boite);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_SetRenderTarget(renderer, NULL);
  for(int pv=personnage->pv; pv>0; pv--){
    SDL_SetRenderTarget(renderer, textureSalle);
    SDL_RenderCopy(renderer, coeurImage, NULL, &coeur);
    SDL_SetRenderTarget(renderer, NULL);
    coeur.x += coeur.w +2;
  }
}
