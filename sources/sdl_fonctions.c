#include "../headers/sdl_fonctions.h"

/**
 * \file sdl_fonctions.c
 * \brief Fichier qui regroupent les fonctions utilisées pour la gestion des graphismes et des évènements (SDL2)
 * \author Marie-Nina MUNAR L2 Info Le Mans
 * \version 5.0
 * \date 17/05/2020
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
  if(TTF_Init () ==  -1){
    fprintf(stderr, "Erreur d’initialisation de TTF_Init : %s\n", TTF_GetError ());
    exit(EXIT_FAILURE);
  }

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

  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){ //Tout les sons du jeu doivent être enregistrés en 44100 de fréquence et en stéréo
    fprintf(stderr, "Erreur d'initialisation de SDL_Mixer : %s\n", Mix_GetError());
    exit(EXIT_FAILURE);
  }

  pourcentageVol = DEFAULTVOLUME;
  Mix_VolumeMusic(pourcentageVol * MIX_MAX_VOLUME);

  Mix_AllocateChannels(NBCHANNELS);
  initialiserChunks();

  SDL_FreeSurface(icon);

}

/**
 * \brief Fonction qui permet de détruire la fenetre et quitter la SDL
 *
 * @param fenetre le pointeur sur le pointeur de la fenetre SDL_Window à détruire
 * @param renderer le pointeur sur le pointeur de la renderer SDL_Renderer à détruire
 */
void quitter_SDL(SDL_Window ** fenetre, SDL_Renderer ** renderer){
  detruireChunks();
  Mix_AllocateChannels(0);
  SDL_DestroyRenderer(*renderer);
  SDL_DestroyWindow(*fenetre);
  TTF_Quit();
  Mix_CloseAudio();
  SDL_Quit();
}

/**
 * \brief Initialise les effets sonores dans le tableau en variable globale
 *
 */
void initialiserChunks(void){
  EffetsSonores[SOUND_TOUCHE]=Mix_LoadWAV("audio/Touche.wav");
  EffetsSonores[SOUND_ARC]=Mix_LoadWAV("audio/Arc.wav");
  EffetsSonores[SOUND_SERPENT]=Mix_LoadWAV("audio/Serpent.wav");
  EffetsSonores[SOUND_SINGE]=Mix_LoadWAV("audio/Singe.wav");
  EffetsSonores[SOUND_VIFPLUME]=Mix_LoadWAV("audio/Vifplume.wav");
  EffetsSonores[SOUND_MURGLACE]=Mix_LoadWAV("audio/Murglace.wav");
  EffetsSonores[SOUND_DOOR]=Mix_LoadWAV("audio/Door.wav");
  EffetsSonores[SOUND_ITEM]=Mix_LoadWAV("audio/Item.wav");
  EffetsSonores[SOUND_MENU]=Mix_LoadWAV("audio/Menu.wav");
}

/**
 * \brief Detruit les effets sonores dans le tableau en variable globale
 *
 */
void detruireChunks(void){
  for(int i=0; i<NBSOUNDS; i++){
    Mix_FreeChunk(EffetsSonores[i]);
    EffetsSonores[i]=NULL;
  }
}

//Pour jouer un son : Mix_PlayChannel(-1, son, 0);

/**
 * \brief Permet de décoder un binaire pour récupérer les sons à jouer
 *
 * @param nbBinaire le binaire à décoder
 */
void ExploitationBinaireSons(unsigned int nbBinaire){
  for(int i=0; i<NBSOUNDS; i++){
    if(nbBinaire & 1)
      Mix_PlayChannel(-1, EffetsSonores[i], 0);
    nbBinaire = nbBinaire >> 1;
  }
}

/**
 * \brief Charge une musique à partir de son emplacement
 *
 * @param path l'emplacement de la musique à charger
 * @return le pointeur sur la musique chargée
 */
Mix_Music * chargerMusique(char * path){
  Mix_Music * musique = Mix_LoadMUS(path);
  return musique;
}

//Pour décharger une musique : Mix_FreeMusic(Mix_Music * musique)

/**
 * \brief Jouer une musique à l'infini
 *
 * @param musique le pointeur sur la musique à jouer
 */
void lancerMusiqueInfini(Mix_Music * musique){
  Mix_PlayMusic(musique, -1);
}

/**
 * \brief Jouer une musique un nombre précis de fois
 *
 * @param musique le pointeur sur la musique à jouer
 * @param nbFois le nombre de fois que l'on joue la musique
 */
void lancerMusiqueNBFois(Mix_Music * musique, int nbFois){
  Mix_PlayMusic(musique, nbFois);
}

/**
 * \brief Passe de la pause au lancement de la musique
 *
 */
void togglePauseMusique(void){
  if(Mix_PausedMusic())
    Mix_ResumeMusic();
  else if(Mix_PlayingMusic())
    Mix_PauseMusic();
}

/**
 * \brief Baisse le son de la musique
 *
 */
void baisserSon(void){
  pourcentageVol-=0.01;
  if(pourcentageVol<0)
    pourcentageVol=0;
  Mix_VolumeMusic(pourcentageVol * MIX_MAX_VOLUME);
}

/**
 * \brief Monte le son de la musique
 *
 */
void MonterSon(void){
  pourcentageVol+=0.01;
  if(pourcentageVol>1)
    pourcentageVol=1;
  Mix_VolumeMusic(pourcentageVol * MIX_MAX_VOLUME);
}

//Pour mettre pause : Mix_PauseMusic();
//Pour reprendre : Mix_ResumeMusic();
//Pour revenir au début : Mix_RewindMusic();
//Pour arrêter la musique : Mix_HaltMusic();


//Ancienne version sans SDL_Mixer
/*audiodata_t * chargerWAVreplay(char * fichier){
  audiodata_t * audiodata = malloc(sizeof(audiodata_t));
  SDL_AudioDeviceID deviceId;
  SDL_AudioSpec wav_spec;
  Uint32 wav_length=0;
  Uint8 *wav_buffer=NULL;
  int success=0;
  SDL_AudioSpec desired;
  desired.freq = 44100;
  desired.format = AUDIO_S16LSB;
  desired.channels = 2;
  desired.samples = 4096;
  desired.callback = Audiocallback;
  desired.userdata = audiodata;

  if (SDL_LoadWAV(fichier, &wav_spec, &wav_buffer, &wav_length)){
    deviceId = SDL_OpenAudioDevice(NULL, 0, &desired, NULL, 0);
    if (!deviceId)
      printf("Le driver audio n'a pas pu être initialise : %s\n", SDL_GetError());
  }
  else
    printf("Le chargement du fichier WAV a echoue: %s\n", SDL_GetError());

  audiodata->deviceId=deviceId;
  audiodata->wav_spec=wav_spec;
  audiodata->wav_length=wav_length;
  audiodata->wav_buffer=wav_buffer;
  audiodata->audioPos=0;
  audiodata->audioLen=audiodata->wav_length;
  audiodata->audioBufferPos=audiodata->wav_buffer;
  audiodata->volume=VOLUMEAUDIO;
  return audiodata;
}

void Audiocallback(void *userdata, Uint8 *stream, int len) {
  audiodata_t * audiodata = (audiodata_t *)(userdata);

  SDL_memset(stream, 0, len);

	if (audiodata->audioLen <= 0 || audiodata->audioPos+len > audiodata->wav_length){
    audiodata->audioPos=0;
    audiodata->audioBufferPos = audiodata->wav_buffer;
    audiodata->audioLen=audiodata->wav_length;
  }
  else if (audiodata->audioPos < audiodata->wav_length){
      //printf("\nCallback du morceau - INFOS OBTENUES :\nWAVLEN : %li\nAWAVBUFFER : %li\nVOLUME : %d\nLEN : %d\nAUDIOLEN : %li\nAUDIOPOS : %li\nAUDIOBUFFERPOS : %li\n\n", audiodata->wav_length, audiodata->wav_buffer, audiodata->volume, len, audiodata->audioLen, audiodata->audioPos, audiodata->audioBufferPos);
      SDL_MixAudioFormat(stream, audiodata->audioBufferPos, audiodata->wav_spec.format, len, audiodata->volume);
      audiodata->audioPos+=len;
      audiodata->audioBufferPos += len;
      audiodata->audioLen -= len;
  }
}


void finMusique(audiodata_t ** audiodata){
  SDL_CloseAudioDevice((*audiodata)->deviceId);
  SDL_FreeWAV((*audiodata)->wav_buffer);
  free(*audiodata);
  *audiodata=NULL;
}
void lectureMusique(audiodata_t * audiodata){
  SDL_PauseAudioDevice(audiodata->deviceId, 0);
}
void pauseMusique(audiodata_t * audiodata){
  SDL_PauseAudioDevice(audiodata->deviceId, 1);
}
void togglePauseMusic(audiodata_t * audiodata){
  SDL_AudioStatus audiostatus = SDL_GetAudioDeviceStatus(audiodata->deviceId);
  if(audiostatus == SDL_AUDIO_PLAYING)
    pauseMusique(audiodata);
  else
    lectureMusique(audiodata);
}*/

/**
 * \brief Fonction qui permet d'initialiser une texture à partir d'une image
 *
 * @param path chaine de caracteres qui correspond au chemin de l'image
 * @param renderer le pointeur sur SDL_Renderer
 * @param estTarget precise si la texture sera utilisée en tant que cible (1) ou non (0)
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
 * @return le pointeur sur la structure personnage initialisée
 */
personnage_t * initialisation_personnage(SDL_Renderer * renderer, position_t positionDepart, position_t positionDepartDelta){
  SDL_Rect * tabhitbox=NULL;
  personnage_t * personnage=malloc(sizeof(personnage_t));
  personnage->pv_max=PVMAX;
  personnage->pv=personnage->pv_max;
  personnage->kb=0;
  personnage->inv=0;
  personnage->hit = FALSE;
  personnage->clign=FREQCLIGN;
  personnage->direction=RIGHT;
  personnage->vit_dep=VITDEPPERS;
  personnage->vit_saut=VITSAUTPERS;
  personnage->vit_chute=VITCHUTEPERS;
  personnage->vit_att=VITATTACKPERS;
  personnage->pos=positionDepart;
  personnage->delta=positionDepartDelta;
  personnage->sprites=initialiser_texture(PLAYERSPRITESPATH, renderer, FALSE);
  personnage->spritesR=initialiser_texture(RENARDSPRITESPATH, renderer, FALSE);
  personnage->spriteActuel.h=HAUTEURSPRITEPERS;
  personnage->spriteActuel.w=LARGEURSPRITEPERS;
  personnage->nbHitbox=2;
  personnage->hitboxActuelle=0;
  personnage->etat = IDLE;
  personnage->newEtat = TRUE;
  personnage->spriteActuel.x=0;
  personnage->spriteActuel.y=personnage->etat * personnage->spriteActuel.w;
  personnage->newItem = FALSE;
  personnage->nbPxSaut = 0;
  personnage->nbSaut=0;
  personnage->jpCd = 0;
  personnage->sounds = 0;
  int * nbAnim = malloc(4*sizeof(int));
  nbAnim[0]=1;
  nbAnim[1]=8;
  nbAnim[2]=8;
  nbAnim[3]=3;
  int * vitAnim = malloc(4*sizeof(int));
  vitAnim[0]=40;
  vitAnim[1]=3;
  vitAnim[2]=5;
  vitAnim[3]=12;
  personnage->nbAnim=nbAnim;
  personnage->vitAnim=vitAnim;
  personnage->evoSprite = personnage->vitAnim[personnage->etat];
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

  tabhitbox = malloc(sizeof(SDL_Rect)*personnage->nbHitbox*NBETATS);
  tabhitbox[IDLE+0].x=OFFSETHITBOXH;
  tabhitbox[IDLE+0].y=1;
  tabhitbox[IDLE+0].h=HAUTEURHITBOXPERS;
  tabhitbox[IDLE+0].w=LARGEURHITBOXPERS;
  tabhitbox[IDLE+NBETATS].x=OFFSETHITBOXR;
  tabhitbox[IDLE+NBETATS].y=0;
  tabhitbox[IDLE+NBETATS].h=HAUTEURHITBOXREN;
  tabhitbox[IDLE+NBETATS].w=LARGEURHITBOXREN;
  tabhitbox[RUNNING+0].x=OFFSETHITBOXH;
  tabhitbox[RUNNING+0].y=1;
  tabhitbox[RUNNING+0].h=HAUTEURHITBOXPERS;
  tabhitbox[RUNNING+0].w=LARGEURHITBOXPERS;
  tabhitbox[RUNNING+NBETATS].x=OFFSETHITBOXR;
  tabhitbox[RUNNING+NBETATS].y=0;
  tabhitbox[RUNNING+NBETATS].h=HAUTEURHITBOXREN;
  tabhitbox[RUNNING+NBETATS].w=LARGEURHITBOXREN;
  tabhitbox[JUMPING+0].x=OFFSETHITBOXH;
  tabhitbox[JUMPING+0].y=1;
  tabhitbox[JUMPING+0].h=HAUTEURHITBOXPERS;
  tabhitbox[JUMPING+0].w=LARGEURHITBOXPERS;
  tabhitbox[JUMPING+NBETATS].x=OFFSETHITBOXR;
  tabhitbox[JUMPING+NBETATS].y=0;
  tabhitbox[JUMPING+NBETATS].h=HAUTEURHITBOXREN;
  tabhitbox[JUMPING+NBETATS].w=LARGEURHITBOXREN;
  tabhitbox[ATTACKING+0].x=OFFSETHITBOXH;
  tabhitbox[ATTACKING+0].y=1;
  tabhitbox[ATTACKING+0].h=HAUTEURHITBOXPERS;
  tabhitbox[ATTACKING+0].w=LARGEURHITBOXPERS;
  tabhitbox[ATTACKING+NBETATS].x=OFFSETHITBOXR;
  tabhitbox[ATTACKING+NBETATS].y=0;
  tabhitbox[ATTACKING+NBETATS].h=HAUTEURHITBOXREN;
  tabhitbox[ATTACKING+NBETATS].w=LARGEURHITBOXREN;
  tabhitbox[FALLING+0].x=OFFSETHITBOXH;
  tabhitbox[FALLING+0].y=1;
  tabhitbox[FALLING+0].h=HAUTEURHITBOXPERS;
  tabhitbox[FALLING+0].w=LARGEURHITBOXPERS;
  tabhitbox[FALLING+NBETATS].x=OFFSETHITBOXR;
  tabhitbox[FALLING+NBETATS].y=0;
  tabhitbox[FALLING+NBETATS].h=HAUTEURHITBOXREN;
  tabhitbox[FALLING+NBETATS].w=LARGEURHITBOXREN;

  personnage->hitbox=tabhitbox;

  return personnage;
}

/**
 * \brief Fonction d'affichage de l'inventaire
 *
 * @param renderer le renderer utilisé
 * @param personnage la structure personnage qui contient l'inventaire
 * @param textureSalle la texture sur laquelle afficher l'inventaire
 */
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
 * \brief Fonction d'affichage du volume sonore
 *
 * @param renderer le renderer utilisé
 */
void afficherVolume(SDL_Renderer * renderer){
  SDL_Rect boite;
  SDL_Rect objPlacement;
  SDL_Texture * pourcentageVolText=NULL;
  char * volume = malloc(5*sizeof(char));
  sprintf(volume, "%3i%c", (int)(pourcentageVol*100), '%');
  int maxw;
  int maxh;
  pourcentageVolText = creerTexte(renderer, "./font/BitCasual.ttf", FONTSIZE, volume, 255, 255, 200);
  free(volume);
  SDL_GetRendererOutputSize(renderer, &maxw, &maxh);
  objPlacement.h = maxh/VOLUMEBOXSIZE;
  objPlacement.w = maxw/VOLUMEBOXSIZE;
  boite.h =  maxh/VOLUMEBOXSIZE +5;
  boite.w =  maxw/VOLUMEBOXSIZE +5;
  boite.x = maxw/100;
  boite.y = maxh - maxh/100 - boite.h;
  objPlacement.x= boite.x + 2;
  objPlacement.y= boite.y + 2;
  SDL_SetRenderDrawColor(renderer, 10, 10, 10, 200);
  SDL_RenderFillRect(renderer, &boite);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderCopy(renderer, pourcentageVolText, NULL, &objPlacement);
  SDL_DestroyTexture(pourcentageVolText);
}



/**
 * \brief Fonction de destruction de la structure personnage
 *
 * @param personnage pointeur sur le pointeur de la structure à détruire
 */
void destroy_personnage(personnage_t ** personnage){
  SDL_DestroyTexture((*personnage)->sprites);
  SDL_DestroyTexture((*personnage)->spritesR);
  SDL_DestroyTexture((*personnage)->inventaireTileset);
  free((*personnage)->nbAnim);
  free((*personnage)->vitAnim);
  free((*personnage)->hitbox);
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
    free((typesMonstre[i]).hitbox);
  }
}

/**
 * \brief Fonction d'initialisation des types de monstres
 *
 * @param renderer le pointeur vers le renderer utilisé par les textures
 */
void initialiser_typeentites(SDL_Renderer * renderer){
  creerTypeEntite();
  for(int i=0; i<NBTYPEMONSTRE; i++){
    typesMonstre[i].sprites = initialiser_texture(typesMonstre[i].path, renderer, FALSE);
  }
}


/**
 * \brief Fonction d'initialisation de la salle
 *
 * @param renderer pointeur vers le renderer utilisé
 * @param nomFichier une chaine de caracteres qui contient le nom du fichier de la salle
 * @param perso le pointeur sur le personnage à mettre dans la salle
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

  SDL_QueryTexture(salle->background, NULL, NULL, &(salle->spriteActuel.w), &(salle->spriteActuel.h));
  salle->spriteActuel.w /= salle->nbsprites;
  salle->spriteActuel.y=0;
  salle->spriteActuel.x=0;

  return salle;
}

/**
 * \brief Fonction qui fait évoluer la salle
 *
 * @param salle pointeur sur la salle à faire évoluer
 */
void evoSalle(salle_t * salle){
  salle->etatanim--;
  if(salle->etatanim<0){
    salle->spriteActuel.x += salle->spriteActuel.w;
    if(salle->spriteActuel.x>=salle->spriteActuel.w*salle->nbsprites)
      salle->spriteActuel.x=0;
    salle->etatanim=salle->animDelay;
  }
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
 * @param textureSalle le pointeur sur la texture de cible
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
    SDL_RenderCopy(renderer, salle->background, &(salle->spriteActuel), NULL);
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
 * @param textureSalle le pointeur sur la texture de cible
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
 * @param textureSalle le pointeur sur la texture de cible
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
 * \brief Fonction d'affichage complet (salle, personnage, etc)
 *
 * @param renderer le pointeur vers le SDL_Renderer à utiliser
 * @param salle le pointeur sur la salle où afficher le joueur et les entités
 * @param personnage le pointeur sur la structure personnage à afficher
 * @param inventaireAffiche un booleen pour savoir si on affiche ou non l'inventaire
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

  SDL_DestroyTexture(textureSalle);
  SDL_DestroyTexture(coeurImage);
}

/**
 * \brief Fonction qui fait évoluer les sprites du personnage (animation)
 *
 * @param perso le pointeur sur la structure personnage à faire évoluer
 */
void miseAjourSprites(personnage_t * perso){
  if((perso->inv || perso->kb)&& perso->hit){
    perso->clign--;
    if(perso->clign<0)
      perso->clign=FREQCLIGN;
  }
  else
    perso->clign=FREQCLIGN;

  if(perso->etat == FALLING){
    perso->spriteActuel.x=(perso->nbAnim[JUMPING] - 1)*(perso->spriteActuel.w);
    perso->spriteActuel.y=JUMPING*(perso->spriteActuel.h);
  }
  else{
    perso->spriteActuel.y=perso->etat * (perso->spriteActuel.h);
  }

  if(perso->newEtat){
    if(perso->etat >= IDLE && perso->etat < FALLING){
      perso->spriteActuel.x=0;
      perso->evoSprite=perso->vitAnim[perso->etat];
    }
    perso->newEtat=FALSE;
  }

  else{
    if(perso->etat >= IDLE && perso->etat < FALLING){
      if(perso->evoSprite<=0){
        perso->spriteActuel.x+=perso->spriteActuel.w;
        perso->evoSprite=perso->vitAnim[perso->etat];
      }
      else (perso->evoSprite)--;
    }
  }
  if(perso->etat >= IDLE && perso->etat < FALLING){
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
  }
  if(perso->forme=='h'){
    if(perso->etat == ATTACKING)
      perso->spriteActuel.w = LARGEURSPRITEPERSATTACK;
    else
      perso->spriteActuel.w = LARGEURSPRITEPERS;
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

    /*else if(entite->etat == FALLING){
      entite->spriteActuel.x=(entite->type->nbAnim[JUMPING] -1)*(entite->spriteActuel.w);
      entite->spriteActuel.y=JUMPING*(entite->spriteActuel.h);
    }*/
    entite->spriteActuel.y=entite->etat * (entite->spriteActuel.h);

    if(entite->newEtat){
      if(entite->etat >= IDLE && entite->etat <= FALLING){
        entite->spriteActuel.x=0;
      }
      entite->newEtat=FALSE;
      //entite->evoSprite=0;
    }
    else{
      if(entite->etat >= IDLE && entite->etat <= FALLING){
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
 * \brief Fonction qui détecte si le Konami code a été rentré et qui applique le cheat code en conséquence
 *
 * @param perso le pointeur vers la structure personnage
 * @param salle le pointeur vers la structure salle
 * @param konami le tableau de caractère correspondant au code en cours (inputs)
 * @param indKon le pointeur vers l'entier qui correspond au nombre de caractères actuellement dans le code entré
 * @param kon le pointeur vers un booléen qui vaut FALSE si le Konami code n'a pas été entré et TRUE si c'est le cas
 */
void konamicode(personnage_t * perso, salle_t * salle, char * konami, int * indKon, boolean_t * kon){
  char verifCode[TAILLEKONAMI] = KONAMICODE;
  int indVerif;
  //tester konamicode
  if(*indKon){
    if(!(*kon)){
      for(indVerif = 0; (indVerif < *indKon) && (konami[indVerif] == verifCode[indVerif]); indVerif++);
      if(*indKon != indVerif){
        *indKon = 0;
        for(int i = 0; i < TAILLEKONAMI; i++)
          konami[i] = '\0';
      }else{
        if(*indKon == TAILLEKONAMI){
          //effectuer code
          printf("Konami code effectué !\n");
          for(int i = 0; i < TAILLE_INVENTAIRE; i++)
              perso->inventaire[i] = 1;
          *kon = TRUE;
          for(int i = 0; i < TAILLEKONAMI; i++)
            konami[i] = '\0';
        }
      }
    }else
      *indKon = 0;
  }
}

/**
 * \brief Fonction qui lance le jeu et gère les inputs, lance les fonctions d'affichage
 *
 * @param fenetre le pointeur vers fenetre utilisée
 * @param renderer le pointeur vers le pointeur sur le renderer utilisé
 * @param mode le mode qui contient des informations sur l'écran
 * @param pJoystick contient les informations sur le joystick ou NULL si aucun n'est utilisé
 * @param fullscreen un entier qui indique si le jeu est en plein écran (1) ou non (0)
 * @return un booleen indiquant si on est sorti parceque le personnage est mort (1) ou non (0)
 */
boolean_t jeu(SDL_Window * fenetre, SDL_Renderer ** renderer, SDL_DisplayMode mode, SDL_Joystick * pJoystick, int fullscreen){
  SDL_Event event;
  Uint32 frameStart;
  int frameTime;
  SDL_Point souris;
  boolean_t messageRes;
  Sint16 x_move;
  Sint16 y_move;
  int inventaireAffiche=0;
  int volumeAffiche=0;
  boolean_t transformationR=FALSE;
  idSounds_t son = 0;
  Mix_Music * musique = NULL;
  //position_t positionDepart;
  //position_t positionDepartDelta;
  //boolean_t windowtouched=FALSE;
  //SDL_Texture * tileset=NULL;
  int save=0;
  salle_t * salle=NULL;
  char nom_bg[100];
  personnage_t * perso=NULL;
  char * salle_nom = NULL;
  boolean_t Gauche = FALSE;
  boolean_t Droite = FALSE;
  boolean_t tryJump = FALSE;
  boolean_t tryAtk = FALSE;
  boolean_t fin=FALSE;
  boolean_t quitter=FALSE;
  boolean_t mort=FALSE;
  boolean_t salleChangee=FALSE;
  boolean_t kon = FALSE;
  char konami[TAILLEKONAMI];
  int indKon = 0;
  for(int i = 0; i < TAILLEKONAMI; i++)
    konami[i] = '\0';

  SDL_SetRenderDrawColor(*renderer,0,0,0,255);
  //SDL_RenderClear(*renderer);
  SDL_RenderFillRect(*renderer, NULL);
  SDL_RenderPresent(*renderer);

  initialiser_typeentites(*renderer);
  //tileset=initialiser_texture(TILESETPATH, *renderer);

  chargerSauvegardeMenu(*renderer, 0, &perso, &salle);

  musique = chargerMusique(LONGAWAYWAV);
  lancerMusiqueInfini(musique);
  quitter=FALSE;
  messageRes=FALSE;

  while(fin==FALSE){
    frameStart = SDL_GetTicks();
    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_QUIT: //Appui sur la croix quitte le programme (avec fenetre de dialogue pour confirmation)
          quitter=TRUE;
          break;
        case SDL_WINDOWEVENT:
          //windowtouched=TRUE;
          destroy_typeentites();
          initialiser_typeentites(*renderer);
          SDL_DestroyTexture(perso->sprites);
          SDL_DestroyTexture(perso->spritesR);
          if(perso->forme=='r'){
            perso->spritesR=initialiser_texture(PLAYERSPRITESPATH, *renderer, FALSE);
            perso->sprites=initialiser_texture(RENARDSPRITESPATH, *renderer, FALSE);
          }
          else{
            perso->sprites=initialiser_texture(PLAYERSPRITESPATH, *renderer, FALSE);
            perso->spritesR=initialiser_texture(RENARDSPRITESPATH, *renderer, FALSE);
          }
          SDL_DestroyTexture(salle->background);
          strcpy(nom_bg,DIRBG);
          strcat(nom_bg, salle->nomFichier);
          nom_bg[strlen(nom_bg) - 3] = '\0';
          strcat(nom_bg, "png");
          salle->background=initialiser_texture(nom_bg, *renderer, FALSE);
          SDL_DestroyTexture(salle->tileset);
          salle->tileset=initialiser_texture(TILESETPATH, *renderer, FALSE);
          //Si la salle devient mouvante (ou autre chose) (blocs), un destroy texture des textures associées est envisageable de la meme façon que pour le personnage
          break;
        case SDL_KEYUP:
          switch(event.key.keysym.sym){
            case SDLK_ESCAPE://Appui sur Echap quitte le programme
              quitter=TRUE;
              break;
            case SDLK_0:
            case SDLK_KP_0:
              save=chargerSauvegardeMenu(*renderer, 0, &perso, &salle);
              printf("Chargement sauvegarde %d\n", save);
              break;
            case SDLK_1:
            case SDLK_KP_1:
              save=chargerSauvegardeMenu(*renderer, 1, &perso, &salle);
              printf("Chargement sauvegarde %d\n", save);
              break;
            case SDLK_2:
            case SDLK_KP_2:
              save=chargerSauvegardeMenu(*renderer, 2, &perso, &salle);
              printf("Chargement sauvegarde %d\n", save);
              break;
            case SDLK_3:
            case SDLK_KP_3:
              save=chargerSauvegardeMenu(*renderer, 3, &perso, &salle);
              printf("Chargement sauvegarde %d\n", save);
              break;
            case SDLK_7:
            case SDLK_KP_7:
              save=sauvegarderMenu(1, perso, salle);
              printf("Sauvegarde dans le slot %d\n", save);
              break;
            case SDLK_8:
            case SDLK_KP_8:
              save=sauvegarderMenu(2, perso, salle);
              printf("Sauvegarde dans le slot %d\n", save);
              break;
            case SDLK_9:
            case SDLK_KP_9:
              save=sauvegarderMenu(3, perso, salle);
              printf("Sauvegarde dans le slot %d\n", save);
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
            case SDLK_n:
              break;
            case SDLK_r:
              transformationR=TRUE;
              break;
            case SDLK_p:
              togglePauseMusique();
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
            case SDLK_PAGEUP:
            case SDLK_KP_PLUS:
              MonterSon();
              volumeAffiche=VOLUMETIME;
              break;
            case SDLK_PAGEDOWN:
            case SDLK_KP_MINUS:
              baisserSon();
              volumeAffiche=VOLUMETIME;
              break;
          }
          break;
        case SDL_MOUSEMOTION :
          SDL_GetMouseState(&(souris.x), &(souris.y));
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
            case 2 : //bouton X manette XBOX
              transformationR=TRUE;
              break;
            case 6 : //bouton select manette XBOX
              quitter=TRUE;
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
        if((perso)->etat == RUNNING){//ajouter par Thomas: on souhaite passer de RUNNING à IDLE mais pas de JUMPING à IDLE ou bien de ATTACKING à IDLE
          (perso)->etat=IDLE;
          perso->newEtat=TRUE;
        }

      depVert(perso, salle, tryJump);

      tryJump=FALSE;

      attaquer(perso,salle,tryAtk);

      tryAtk=FALSE;

      evolution(perso,salle);
      evoSalle(salle);

      ExploitationBinaireSons(perso->sounds);

      if(perso->pv == 0){
        fin=TRUE;
        mort=TRUE;
        SDL_Delay(1000);
      }

      if(perso->newItem){
        inventaireAffiche=INVENTAIRETIME-1;
        perso->newItem = FALSE;
      }


      if(transformationR){
        transformation(perso, salle);
        transformationR=FALSE;
      }

      miseAjourSprites(perso);
      miseAjourSpritesEntites(salle);

      if(salleChangee){
        ecranNoir(*renderer,150);
        salleChangee=FALSE;
      }

      if(quitter){
        messageRes=menuConfirmation(*renderer, "Voulez-vous quitter la partie ?", 2,5);
        if(messageRes)
          fin=TRUE;
        else
          quitter=FALSE;
      }
      SDL_SetRenderDrawColor(*renderer,0,0,0,255);
      //SDL_RenderClear(*renderer);
      SDL_RenderFillRect(*renderer, NULL);
      affichage_complet(*renderer, salle, perso, &inventaireAffiche);

      if(volumeAffiche > 0){
        afficherVolume(*renderer);
        if(volumeAffiche <= VOLUMETIME)
          (volumeAffiche)--;
      }

      SDL_RenderPresent(*renderer);

      frameTime = SDL_GetTicks() - frameStart;
      if(frameTime < FRAMEDELAY){
        SDL_Delay(FRAMEDELAY - frameTime);
      }
    }

    destroy_salle(&salle);
    destroy_personnage(&perso);
    destroy_typeentites();
    SDL_SetRenderDrawColor(*renderer,0,0,0,255);
    SDL_RenderClear(*renderer);
    SDL_RenderFillRect(*renderer, NULL);
    SDL_RenderPresent(*renderer);
    //SDL_DestroyTexture(tileset);
    Mix_HaltMusic();
    if(musique != NULL)
      Mix_FreeMusic(musique);
    return mort;
}

/**
 * \brief Fonction qui créer le menu de démarrage du jeu
 *
 * @param renderer le pointeur sur le renderer utilisé
 * @return le pointeur sur le menu créé
 */
menu_t * creerMenuDemarrage(SDL_Renderer * renderer){
  menu_t * menu = malloc(sizeof(menu_t));

  //Général :
  menu->nbBoutons=3;
  menu->idBoutonChoisi=2;
  menu->idBoutonValide=0;
  menu->nbTextes=1;
  menu->etiquette = "Menu Principal";
  menu->tabTextes = malloc(menu->nbTextes * sizeof(menu_texte_t));
  menu->tabBoutons = malloc(menu->nbBoutons * sizeof(menu_bouton_t));

  //Etiquettes :
  menu->tabTextes[0].etiquette=NOM_JEU;
  menu->tabBoutons[0].etiquette="Commencer";
  menu->tabBoutons[1].etiquette="Continuer";
  menu->tabBoutons[2].etiquette="Quitter";

  //Fond :
  menu->nbSprites = 7;
  menu->spriteActuel.x = 0;
  menu->spriteActuel.y = 0;
  menu->animDelay = 30;
  menu->etatanim = menu->animDelay;

  //Texte :
  for(int i=0; i<menu->nbTextes; i++)
    menu->tabTextes[i].id=i+1;

  //Boutons :
  for(int i=0; i<menu->nbBoutons; i++){
    menu->tabBoutons[i].id=i+1+menu->nbTextes;
    menu->tabBoutons[i].etat=RELAXED;
  }

  //Textures :
  creerTexturesMenuDemarrage(renderer, menu);

  return menu;
}

/**
 * \brief Fonction qui créer le menu in-game du jeu
 *
 * @param renderer le pointeur sur le renderer utilisé
 * @return le pointeur sur le menu créé
 */
menu_t * creerMenuInGame(SDL_Renderer * renderer){
  menu_t * menu = malloc(sizeof(menu_t));
  //A FAIRE
  return menu;
}

/**
 * \brief Fonction qui créer un menu de confirmation
 *
 * @param renderer le pointeur sur le renderer utilisé
 * @param question la question posée
 * @return le pointeur sur le menu créé
 */
menu_t * creerMenuConfirmation(SDL_Renderer * renderer, char * question){
  menu_t * menu = malloc(sizeof(menu_t));

  //Général :
  menu->nbBoutons=2;
  menu->idBoutonChoisi=3;
  menu->idBoutonValide=0;
  menu->nbTextes=1;
  menu->etiquette = "Menu Confirmation";
  menu->tabTextes = malloc(menu->nbTextes * sizeof(menu_texte_t));
  menu->tabBoutons = malloc(menu->nbBoutons * sizeof(menu_bouton_t));

  //Etiquette :
  menu->tabTextes[0].etiquette=question;
  menu->tabBoutons[0].etiquette="Oui";
  menu->tabBoutons[1].etiquette="Non";

  //Fond :
  menu->nbSprites = 7;
  menu->spriteActuel.x = 0;
  menu->spriteActuel.y = 0;
  menu->animDelay = 30;
  menu->etatanim = menu->animDelay;

  //Texte :
  for(int i=0; i<menu->nbTextes; i++)
    menu->tabTextes[i].id=i+1;

  //Boutons :
  for(int i=0; i<menu->nbBoutons; i++){
    menu->tabBoutons[i].id=i+1+menu->nbTextes;
    menu->tabBoutons[i].etat=RELAXED;
  }

  //Textures :
  creerTexturesMenuConfirmation(renderer, menu);

  return menu;
}

/**
 * \brief Fonction qui détruit un menu
 *
 * @param menu le pointeur sur le pointeur sur le menu à detruire
 */
void detruireMenu(menu_t ** menu){
  detruireTexturesMenu(*menu);
  free((*menu)->tabTextes);
  free((*menu)->tabBoutons);
  free(*menu);
  *menu=NULL;
}

/**
 * \brief Fonction qui détruit les textures d'un menu
 *
 * @param menu le pointeur sur le menu qui contient les textures
 */
void detruireTexturesMenu(menu_t * menu){
  for(int i=0; i<menu->nbBoutons; i++){
    for(int j=0; j<NBETATSBOUTONS; j++)
      if(menu->tabBoutons[i].texture[j] != NULL)
        SDL_DestroyTexture(menu->tabBoutons[i].texture[j]);
    free(menu->tabBoutons[i].texture);
  }
  for(int j=0; j<menu->nbTextes; j++){
    SDL_DestroyTexture(menu->tabTextes[j].texture);
  }
  SDL_DestroyTexture(menu->fond);
}

/**
 * \brief Fonction qui créer les textures du menu de démarrage du jeu
 *
 * @param renderer le pointeur sur le renderer utilisé
 * @param menu le pointeur sur le menu qui va contenir les textures
 */
void creerTexturesMenuDemarrage(SDL_Renderer * renderer, menu_t * menu){
  int fondw=0;
  int fondh=0;
  menu->fond = initialiser_texture("./sprites/menu/menu.png", renderer, FALSE);
  SDL_QueryTexture(menu->fond, NULL, NULL, &fondw, &fondh);
  fondw /= menu->nbSprites;
  menu->spriteActuel.w = fondw;
  menu->spriteActuel.h = fondh;

  menu->tabTextes[0].texture = creerTexte(renderer, "./font/PixelMordred.ttf", FONTSIZE, menu->tabTextes[0].etiquette, 255, 255, 200);
  SDL_QueryTexture(menu->tabTextes[0].texture, NULL, NULL, &(menu->tabTextes[0].emplacement.w), &(menu->tabTextes[0].emplacement.h));
  menu->tabTextes[0].emplacement.x=0;
  menu->tabTextes[0].emplacement.y=0;
  if(menu->tabTextes[0].emplacement.w>=fondw)
    menu->tabTextes[0].emplacement.w==fondw-1;
  if(menu->tabTextes[0].emplacement.h>=fondh)
    menu->tabTextes[0].emplacement.h=fondh-1;

  menu->tabBoutons[0].texture = malloc(NBETATSBOUTONS * sizeof(SDL_Texture *));
  menu->tabBoutons[0].texture[RELAXED] = initialiser_texture("./sprites/bouton/commencer/nouvelle_partie.png", renderer, FALSE);
  menu->tabBoutons[0].texture[HIGHLIGHTED] = initialiser_texture("./sprites/bouton/commencer/nouvelle_partie_selectionnee.png", renderer, FALSE);
  menu->tabBoutons[0].texture[PRESSED] = initialiser_texture("./sprites/bouton/commencer/nouvelle_partie_clique.png", renderer, FALSE);
  menu->tabBoutons[0].texture[UNAVAILABLE] = initialiser_texture("./sprites/bouton/commencer/nouvelle_partie_impossible.png", renderer, FALSE);
  SDL_QueryTexture(menu->tabBoutons[0].texture[RELAXED], NULL, NULL, &(menu->tabBoutons[0].emplacement.w), &(menu->tabBoutons[0].emplacement.h));
  menu->tabBoutons[0].emplacement.x=0;
  menu->tabBoutons[0].emplacement.y=0;
  if(menu->tabBoutons[0].emplacement.w>=fondw)
    menu->tabBoutons[0].emplacement.w=fondw-1;
  if(menu->tabBoutons[0].emplacement.h>=fondh)
    menu->tabBoutons[0].emplacement.h=fondh-1;

  menu->tabBoutons[1].texture = malloc(NBETATSBOUTONS * sizeof(SDL_Texture *));
  menu->tabBoutons[1].texture[RELAXED] = initialiser_texture("./sprites/bouton/continuer/continuer.png", renderer, FALSE);
  menu->tabBoutons[1].texture[HIGHLIGHTED] = initialiser_texture("./sprites/bouton/continuer/continuer_selectionne.png", renderer, FALSE);
  menu->tabBoutons[1].texture[PRESSED] = initialiser_texture("./sprites/bouton/continuer/continuer_clique.png", renderer, FALSE);
  menu->tabBoutons[1].texture[UNAVAILABLE] = initialiser_texture("./sprites/bouton/continuer/continuer_impossible.png", renderer, FALSE);
  SDL_QueryTexture(menu->tabBoutons[1].texture[RELAXED], NULL, NULL, &(menu->tabBoutons[1].emplacement.w), &(menu->tabBoutons[1].emplacement.h));
  menu->tabBoutons[1].emplacement.x=0;
  menu->tabBoutons[1].emplacement.y=0;
  if(menu->tabBoutons[1].emplacement.w>=fondw)
    menu->tabBoutons[1].emplacement.w=fondw-1;
  if(menu->tabBoutons[1].emplacement.h>=fondh)
    menu->tabBoutons[1].emplacement.h=fondh-1;

  menu->tabBoutons[2].texture = malloc(NBETATSBOUTONS * sizeof(SDL_Texture *));
  menu->tabBoutons[2].texture[RELAXED] = initialiser_texture("./sprites/bouton/quitter/quitter.png", renderer, FALSE);
  menu->tabBoutons[2].texture[HIGHLIGHTED] = initialiser_texture("./sprites/bouton/quitter/quitter_selectionne.png", renderer, FALSE);
  menu->tabBoutons[2].texture[PRESSED] = initialiser_texture("./sprites/bouton/quitter/quitter_clique.png", renderer, FALSE);
  menu->tabBoutons[2].texture[UNAVAILABLE] = initialiser_texture("./sprites/bouton/quitter/quitter_impossible.png", renderer, FALSE);
  SDL_QueryTexture(menu->tabBoutons[2].texture[RELAXED], NULL, NULL, &(menu->tabBoutons[2].emplacement.w), &(menu->tabBoutons[2].emplacement.h));
  if(menu->tabBoutons[2].emplacement.w>=fondw)
    menu->tabBoutons[2].emplacement.w=fondw-1;
  if(menu->tabBoutons[2].emplacement.h>=fondh)
    menu->tabBoutons[2].emplacement.h=fondh-1;

}

/**
 * \brief Fonction qui créer les textures d'un menu de confirmation'
 *
 * @param renderer le pointeur sur le renderer utilisé
 * @param menu le pointeur sur le menu qui va contenir les textures
 */
void creerTexturesMenuConfirmation(SDL_Renderer * renderer, menu_t * menu){
  int fondw=0;
  int fondh=0;
  menu->fond = initialiser_texture("./sprites/menu/menu.png", renderer, FALSE);
  SDL_QueryTexture(menu->fond, NULL, NULL, &fondw, &fondh);
  fondw /= menu->nbSprites;
  menu->spriteActuel.w = fondw;
  menu->spriteActuel.h = fondh;


  menu->tabTextes[0].texture = creerTexte(renderer, "./font/BitCasual.ttf", FONTSIZE, menu->tabTextes[0].etiquette, 255, 255, 100);
  SDL_QueryTexture(menu->tabTextes[0].texture, NULL, NULL, &(menu->tabTextes[0].emplacement.w), &(menu->tabTextes[0].emplacement.h));
  menu->tabTextes[0].emplacement.x=0;
  menu->tabTextes[0].emplacement.y=0;
  if(menu->tabTextes[0].emplacement.w>=fondw)
    menu->tabTextes[0].emplacement.w=fondw-1;
  if(menu->tabTextes[0].emplacement.h>=fondh)
    menu->tabTextes[0].emplacement.h=fondh-1;

  menu->tabBoutons[0].texture = malloc(NBETATSBOUTONS * sizeof(SDL_Texture *));
  menu->tabBoutons[0].texture[RELAXED] = initialiser_texture("./sprites/bouton/oui/oui.png", renderer, FALSE);
  menu->tabBoutons[0].texture[HIGHLIGHTED] = initialiser_texture("./sprites/bouton/oui/oui_selectionne.png", renderer, FALSE);
  menu->tabBoutons[0].texture[PRESSED] = initialiser_texture("./sprites/bouton/oui/oui_clique.png", renderer, FALSE);
  menu->tabBoutons[0].texture[UNAVAILABLE] = NULL;
  SDL_QueryTexture(menu->tabBoutons[0].texture[RELAXED], NULL, NULL, &(menu->tabBoutons[0].emplacement.w), &(menu->tabBoutons[0].emplacement.h));
  menu->tabBoutons[0].emplacement.x=0;
  menu->tabBoutons[0].emplacement.y=0;
  if(menu->tabBoutons[0].emplacement.w>=fondw)
    menu->tabBoutons[0].emplacement.w=fondw-1;
  if(menu->tabBoutons[0].emplacement.h>=fondh)
    menu->tabBoutons[0].emplacement.h=fondh-1;

  menu->tabBoutons[1].texture = malloc(NBETATSBOUTONS * sizeof(SDL_Texture *));
  menu->tabBoutons[1].texture[RELAXED] = initialiser_texture("./sprites/bouton/non/non.png", renderer, FALSE);
  menu->tabBoutons[1].texture[HIGHLIGHTED] = initialiser_texture("./sprites/bouton/non/non_selectionne.png", renderer, FALSE);
  menu->tabBoutons[1].texture[PRESSED] = initialiser_texture("./sprites/bouton/non/non_clique.png", renderer, FALSE);
  menu->tabBoutons[1].texture[UNAVAILABLE] = NULL;
  SDL_QueryTexture(menu->tabBoutons[1].texture[RELAXED], NULL, NULL, &(menu->tabBoutons[1].emplacement.w), &(menu->tabBoutons[1].emplacement.h));
  menu->tabBoutons[1].emplacement.x=0;
  menu->tabBoutons[1].emplacement.y=0;
  if(menu->tabBoutons[1].emplacement.w>=fondw)
    menu->tabBoutons[1].emplacement.w=fondw-1;
  if(menu->tabBoutons[1].emplacement.h>=fondh)
    menu->tabBoutons[1].emplacement.h=fondh-1;
}

/**
 * \brief Fonction qui lance un menu de confirmation
 *
 * @param renderer le pointeur sur le renderer utilisé
 * @param question la question posée au joueur
 * @param tailleTexte la taille du texte dans le menu (multiplicateur)
 * @param tailleBoutons la taille des boutons dans le menu (multiplicateur)
 */
boolean_t menuConfirmation(SDL_Renderer * renderer, char * question, int tailleTexte, int tailleBoutons){

  SDL_Event event;
  Uint32 frameStart;
  int frameTime;
  SDL_Point souris;
  souris.x=0;
  souris.y=0;
  int direction=0;
  int clique=0;
  int volumeAffiche=0;
  boolean_t bougeSouris=FALSE;
  Sint16 x_move;
  Sint16 y_move;
  boolean_t fin=FALSE;
  boolean_t quitter=FALSE;

  menu_t * menu=NULL;

  menu = creerMenuConfirmation(renderer, question);
  SDL_GetMouseState(&(souris.x), &(souris.y));
  while(!fin){

    frameStart = SDL_GetTicks();
    direction=0;
    quitter=FALSE;
    bougeSouris = FALSE;
    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_QUIT: //Appui sur la croix quitte le programme
          quitter=TRUE;
          fin=TRUE;
          break;
        case SDL_WINDOWEVENT:
          detruireTexturesMenu(menu);
          creerTexturesMenuConfirmation(renderer, menu);
          break;
        case SDL_KEYUP:
          switch(event.key.keysym.sym){
            case SDLK_ESCAPE://Appui sur Echap cancel
              quitter=FALSE;
              fin=TRUE;
              break;
            case SDLK_UP:
            case SDLK_z:
            case SDLK_LEFT:
            case SDLK_q:
              direction=-1;
              break;
            case SDLK_RIGHT:
            case SDLK_d:
            case SDLK_DOWN:
            case SDLK_s:
              direction=1;
              break;
            case SDLK_p:
              togglePauseMusique();
              break;
            case SDLK_RETURN:
              clique=2;
              break;
          }
          break;
        case SDL_KEYDOWN:
          switch(event.key.keysym.sym){
            case SDLK_LEFT:
            case SDLK_q:
              break;
            case SDLK_RIGHT:
            case SDLK_d:
              break;
            case SDLK_UP:
            case SDLK_z:
            case SDLK_SPACE:
                break;
            case SDLK_DOWN:
            case SDLK_s:
              break;
            case SDLK_e:
              break;
            case SDLK_PAGEUP:
            case SDLK_KP_PLUS:
              MonterSon();
              volumeAffiche = VOLUMETIME;
              break;
            case SDLK_PAGEDOWN:
            case SDLK_KP_MINUS:
              baisserSon();
              volumeAffiche = VOLUMETIME;
              break;
            case SDLK_RETURN:
              clique=1;
              break;
          }
          break;
        case SDL_MOUSEMOTION :
          bougeSouris = TRUE;
          break;
        case SDL_MOUSEBUTTONUP:
          clique=2;
          break;
        case SDL_MOUSEBUTTONDOWN:
          clique=1;
          break;
        case SDL_JOYBUTTONDOWN :
        switch(event.jbutton.button){
            case 0 : //bouton A manette XBOX
            case 7 : //bouton Start manette XBOX
              clique=1;
              break;
          }
        break;
        case SDL_JOYBUTTONUP :
          switch(event.jbutton.button){
            case 0 : //bouton A manette XBOX
            case 7 : //bouton Start manette XBOX
              clique=2;
              break;
            case 6 : //bouton Select manette XBOX
              quitter=FALSE;
              fin=TRUE;
              break;
            }
          break;
        case SDL_JOYAXISMOTION :
          break;
        /*case SDL_JOYBALLMOTION :
          break;*/
        case SDL_JOYHATMOTION :
          switch(event.jhat.value){
            case SDL_HAT_CENTERED:
              break;
            case SDL_HAT_LEFT:
            case SDL_HAT_UP:
              direction=-1;
              break;
            case SDL_HAT_RIGHT:
            case SDL_HAT_DOWN:
              direction=1;
              break;
          }
      }
    }

    if(menu->idBoutonValide>menu->nbTextes){
      Mix_PlayChannel(-1, EffetsSonores[SOUND_MENU], 0);
      fin=TRUE;
    }

    if(menu->idBoutonValide==2)
      quitter = TRUE;
    else if(menu->idBoutonValide==3)
      quitter = FALSE;

    afficher_menu(renderer, menu, tailleTexte, tailleBoutons, FALSE);

    if(volumeAffiche > 0){
      afficherVolume(renderer);
      if(volumeAffiche <= VOLUMETIME)
        (volumeAffiche)--;
    }

    SDL_RenderPresent(renderer);

    SDL_GetMouseState(&(souris.x), &(souris.y));

    TouchesMenu(direction, souris, bougeSouris, menu);

    menu->idBoutonValide=evoMenu(menu, clique);

    if(clique == 2)
      clique=0;

    frameTime = SDL_GetTicks() - frameStart;
    if(frameTime < FRAMEDELAY){
      SDL_Delay(FRAMEDELAY - frameTime);
    }

  }

  detruireMenu(&menu);
  return quitter;
}

/**
 * \brief Fonction d'affichage de la vie sous forme de jauge
 *
 * @param renderer le pointeur vers le SDL_Renderer à utiliser
 * @param personnage le pointeur sur le personnage (points de vie)
 * @param textureSalle le pointeur sur la texture de cible
 */
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

/**
 * \brief Fonction d'affichage de la vie sous forme de coeurs
 *
 * @param renderer le pointeur vers le SDL_Renderer à utiliser
 * @param personnage le pointeur sur le personnage (points de vie)
 * @param coeurImage la texture de l'image d'un coeur
 * @param textureSalle le pointeur sur la texture de cible
 */
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

/**
 * \brief Fonction qui lance un gameover
 * @param fenetre le pointeur vers fenetre utilisée
 * @param renderer le pointeur vers le pointeur sur le renderer utilisé
 * @param mode le mode qui contient des informations sur l'écran
 * @param pJoystick contient les informations sur le joystick ou NULL si aucun n'est utilisé
 * @param fullscreen un entier qui indique si le jeu est en plein écran (1) ou non (0)
*/
void gameover(SDL_Window * fenetre, SDL_Renderer * renderer, SDL_DisplayMode mode, SDL_Joystick * pJoystick, int fullscreen){
  SDL_Rect Rect_dest;
  int maxw;
  int maxh;
  int textew;
  int texteh;
  int mult;
  Mix_Music * musique = NULL;
  int volumeAffiche=0;
  SDL_Event event;
  Uint32 frameStart;
  int frameTime;
  boolean_t fin=FALSE;
  SDL_Texture * texte=creerTexte(renderer, "./font/BitCasual.ttf", FONTSIZE*2, "GAME OVER", 255, 255, 200);

  musique = chargerMusique(FUTURISTICWAV);
  lancerMusiqueInfini(musique);

  while(!fin){
    frameStart = SDL_GetTicks();
    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_KEYUP :
          switch(event.key.keysym.sym){
            case SDLK_RETURN :
            case SDLK_ESCAPE :
            case SDLK_SPACE :
            fin=TRUE;
            break;
            case SDLK_p :
              togglePauseMusique();
              break;
          }
          break;
        case SDL_KEYDOWN :
          switch(event.key.keysym.sym){
            case SDLK_PAGEUP:
            case SDLK_KP_PLUS:
              MonterSon();
              volumeAffiche=VOLUMETIME;
              break;
            case SDLK_PAGEDOWN:
            case SDLK_KP_MINUS:
              baisserSon();
              volumeAffiche=VOLUMETIME;
              break;
          }
        case SDL_JOYBUTTONUP:
          switch(event.jbutton.button){
            case 0 : //bouton A manette XBOX
            case 6 : //bouton Select de la manette XBOX
            case 7 : //bouton Start manette XBOX
              fin = TRUE;
              break;
            }
          break;
        case SDL_MOUSEBUTTONUP:
        case SDL_QUIT:
          fin=TRUE;
          break;
      }
    }
    SDL_SetRenderDrawColor(renderer, 20,0,0,255);
    SDL_RenderFillRect(renderer,NULL);
    SDL_GetRendererOutputSize(renderer, &maxw, &maxh);
    SDL_QueryTexture(texte, NULL, NULL, &textew, &texteh);
    mult = (2*(maxw + maxh))/(3*(textew + texteh));
    textew *= mult;
    texteh *= mult;
    Rect_dest.x = maxw/2 - textew/2;
    if(Rect_dest.x < 0)
      Rect_dest.x = 0;
    Rect_dest.y = maxh/2 - texteh/2;
    if(Rect_dest.y < 0)
      Rect_dest.y = 0;
    Rect_dest.w = textew < maxw ? textew : maxw;
    Rect_dest.h = texteh < maxh ? texteh : maxh;

    SDL_RenderCopy(renderer, texte, NULL, &Rect_dest);

    if(volumeAffiche > 0){
      afficherVolume(renderer);
      if(volumeAffiche <= VOLUMETIME)
        (volumeAffiche)--;
    }

    SDL_RenderPresent(renderer);

    frameTime = SDL_GetTicks() - frameStart;
    if(frameTime < FRAMEDELAY){
      SDL_Delay(FRAMEDELAY - frameTime);
    }

  }
  Mix_HaltMusic();
  if(musique != NULL)
    Mix_FreeMusic(musique);
  SDL_DestroyTexture(texte);
  SDL_Delay(500);
}

/**
 * \brief Fonction qui créé une police
 * @param path de la police ttf
 * @param taille la taille de la police
 * @return un pointeur sur la police créée
*/
TTF_Font * creerPolice(char * path, int taille){
  TTF_Font * font = TTF_OpenFont(path, taille);
  return font;
}

/**
 * \brief Fonction qui détruit une police
 * @param font le pointeur sur le pointeur de la police à detruire
*/
void detruirePolice(TTF_Font ** font){
  TTF_CloseFont(*font);
  *font=NULL;
}

/**
 * \brief Fonction qui créé un texte
 * @param renderer le renderer utilisé
 * @param Fontpath le chemin vers la police utilisée
 * @param taille la taille de la police
 * @param texte le texte à créer
 * @param r la donnée entière du rouge
 * @param g la donnée entière du vert
 * @param b la donnée entière du bleu
 * @return un pointeur sur la texture créée
*/
SDL_Texture * creerTexte(SDL_Renderer * renderer, char * Fontpath, int taille, char* texte, int r, int g, int b){
  SDL_Color color = {r,g,b};
  TTF_Font * font=creerPolice(Fontpath, taille);
  SDL_Surface * surface = TTF_RenderText_Solid(font, texte, color);
  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_FreeSurface(surface);
  detruirePolice(&font);
  return texture;
}

/**
 * \brief Fonction qui affiche un menu
 * @param renderer le renderer utilisé
 * @param menu le menu à afficher
 * @param tailleTexte la taille du texte (multiplicateur)
 * @param tailleBoutons la taille des boutons (multiplicateur)
 * @param sens si les boutons du menu sont affichés verticalement (1) ou horizontalement (0)
*/
void afficher_menu(SDL_Renderer * renderer, menu_t * menu, int tailleTexte, int tailleBoutons, boolean_t sens){
  int maxw=0;
  int maxh=0;
  int fondw=0;
  int fondh=0;
  float mult=1.0;
  int i,j;
  float ratioFenetre, ratioMenu;
  SDL_Rect Rect_dest_fond;
  SDL_Rect Rect_dest_obj;

  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
  SDL_RenderFillRect(renderer,NULL);

  SDL_GetRendererOutputSize(renderer, &maxw, &maxh);
  fondw=menu->spriteActuel.w;
  fondh=menu->spriteActuel.h;

  ratioFenetre = (maxw * 1.0) / (maxh * 1.0);
  ratioMenu = (fondw * 1.0) / (fondh * 1.0);

  if(ratioFenetre<ratioMenu){
    Rect_dest_fond.w = maxw;
    Rect_dest_fond.h = Rect_dest_fond.w /ratioMenu;
  }
  else{
    Rect_dest_fond.h = maxh;
    Rect_dest_fond.w = Rect_dest_fond.h * ratioMenu;
  }

  Rect_dest_fond.x = (maxw - Rect_dest_fond.w)/2 ;
  Rect_dest_fond.y = (maxh - Rect_dest_fond.h)/2 ;

  SDL_RenderCopy(renderer, menu->fond, &(menu->spriteActuel), &Rect_dest_fond);

  fondw=Rect_dest_fond.w;
  fondh=Rect_dest_fond.h;
  for(i=0; i<menu->nbTextes; i++){
    Rect_dest_obj.w=menu->tabTextes[i].emplacement.w;
    Rect_dest_obj.h=menu->tabTextes[i].emplacement.h;
    mult = (fondw + fondh)/(1.0*(tailleTexte *(Rect_dest_obj.w + Rect_dest_obj.h)));

    Rect_dest_obj.w *= mult;
    Rect_dest_obj.h *= mult;

    if(Rect_dest_obj.w >= fondw)
      Rect_dest_obj.w = fondw-1;
    if(Rect_dest_obj.h >= fondh)
      Rect_dest_obj.h = fondh-1;

    if(Rect_dest_obj.w <1)
      Rect_dest_obj.w = 1;
    if(Rect_dest_obj.h <1)
      Rect_dest_obj.h = 1;

    Rect_dest_obj.x = 1* fondw/3 - Rect_dest_obj.w/2 + Rect_dest_fond.x;
    if(Rect_dest_obj.x < Rect_dest_fond.x)
      Rect_dest_obj.x = Rect_dest_fond.x;
    Rect_dest_obj.y = (menu->tabTextes[i].id) * (fondh/(menu->nbBoutons+menu->nbTextes+1) + Rect_dest_obj.h/(menu->nbTextes*2)) - Rect_dest_obj.h/(menu->nbTextes) + Rect_dest_fond.y;
    if(Rect_dest_obj.y < Rect_dest_fond.y)
      Rect_dest_obj.y = Rect_dest_fond.y;
    menu->tabTextes[i].emplacement.w = Rect_dest_obj.w;
    menu->tabTextes[i].emplacement.h = Rect_dest_obj.h;
    menu->tabTextes[i].emplacement.x = Rect_dest_obj.x;
    menu->tabTextes[i].emplacement.y = Rect_dest_obj.y;
    SDL_RenderCopy(renderer, menu->tabTextes[i].texture, NULL, &(Rect_dest_obj));
  }

  for(j=0; j<menu->nbBoutons; j++){
    Rect_dest_obj.w=menu->tabBoutons[j].emplacement.w;
    Rect_dest_obj.h=menu->tabBoutons[j].emplacement.h;

    mult = (fondw + fondh)/(1.0*(tailleBoutons *(Rect_dest_obj.w + Rect_dest_obj.h)));

    Rect_dest_obj.w *= mult;
    Rect_dest_obj.h *= mult;

    if(Rect_dest_obj.w >= fondw)
      Rect_dest_obj.w = fondw-1;
    if(Rect_dest_obj.h >= fondh)
      Rect_dest_obj.h = fondh-1;

    if(Rect_dest_obj.w <1)
      Rect_dest_obj.w = 1;
    if(Rect_dest_obj.h <1)
      Rect_dest_obj.h = 1;

    if(sens){
      Rect_dest_obj.x = fondw/2 - Rect_dest_obj.w/2 + Rect_dest_fond.x;
      if(Rect_dest_obj.x < Rect_dest_fond.x)
        Rect_dest_obj.x = Rect_dest_fond.x;
      Rect_dest_obj.y = ((fondh - (menu->tabTextes[menu->nbTextes-1].emplacement.y - Rect_dest_fond.y + menu->tabTextes[menu->nbTextes-1].emplacement.h) - (menu->nbBoutons*Rect_dest_obj.h))/(menu->nbBoutons+1))*(j+1) + Rect_dest_obj.h*j + (menu->tabTextes[menu->nbTextes-1].emplacement.y + menu->tabTextes[menu->nbTextes-1].emplacement.h);
      if(Rect_dest_obj.y < Rect_dest_fond.y)
        Rect_dest_obj.y = Rect_dest_fond.y;
    }
    else{
        Rect_dest_obj.x = ((fondw - (menu->nbBoutons*Rect_dest_obj.w))/(menu->nbBoutons+1))*(j+1) + Rect_dest_obj.w*j + Rect_dest_fond.x;
      if(Rect_dest_obj.x < Rect_dest_fond.x)
        Rect_dest_obj.x = Rect_dest_fond.x;
      Rect_dest_obj.y = 2* fondh/3 - Rect_dest_obj.h/2 + Rect_dest_fond.y;
      if(Rect_dest_obj.y < Rect_dest_fond.y)
        Rect_dest_obj.y = Rect_dest_fond.y;
    }
    menu->tabBoutons[j].emplacement.w = Rect_dest_obj.w;
    menu->tabBoutons[j].emplacement.h = Rect_dest_obj.h;
    menu->tabBoutons[j].emplacement.x = Rect_dest_obj.x;
    menu->tabBoutons[j].emplacement.y = Rect_dest_obj.y;
    SDL_RenderCopy(renderer, menu->tabBoutons[j].texture[menu->tabBoutons[j].etat], NULL, &(Rect_dest_obj));
  }
}

/**
 * \brief Fonction qui gère si il y a changement de boutons ou selection dans le menu
 * @param direction la direction du changement de bouton (gauche ou droite, haut ou bas, avancer ou reculer)
 * @param souris le point qui correspond à la position du pointeur de la souris sur l'écran
 * @param bougeSouris un booléenqui indique si la souris a bougé ou non
 * @param menu le menu à faire évoluer
*/
void TouchesMenu(int direction, SDL_Point souris, boolean_t bougeSouris, menu_t * menu){ //direction vaut -1 pour haut, 1 pour bas et 0 pour immobile, si immobile, la souris est prise en compte
  if(direction == 1){
    menu->idBoutonChoisi +=1;
    if(menu->idBoutonChoisi >= menu->nbTextes + menu->nbBoutons +1)
      menu->idBoutonChoisi = menu->nbTextes + 1;
  }
  else if(direction == -1){
    menu->idBoutonChoisi -=1;
    if(menu->idBoutonChoisi <= menu->nbTextes)
      menu->idBoutonChoisi = menu->nbTextes + menu->nbBoutons;
  }
  else{
    if(bougeSouris){
      for(int i=0; i<menu->nbBoutons; i++){
        if(SDL_PointInRect(&souris, &(menu->tabBoutons[i].emplacement))){
          menu->idBoutonChoisi = menu->tabBoutons[i].id;
        }
      }
    }
  }
}

/**
 * \brief Fonction qui gère si un bouton à été validé
 * @param menu le menu à faire évoluer
 * @param clique qui indique si le bouton a été cliqué ou non
 * @return l'id du bouton validé ou 0 si aucun n'a été validé
*/
int evoMenu(menu_t * menu, int clique){
  //Boutons :
  int idBoutonValide=0;
  for(int i=0; i<menu->nbBoutons; i++){
    menu->tabBoutons[i].etat=RELAXED;
    if(menu->idBoutonChoisi>0){
      if(menu->idBoutonChoisi == menu->tabBoutons[i].id){
        if(clique==1){
          menu->tabBoutons[i].etat=PRESSED;
        }
        else if(clique==2){
          idBoutonValide=menu->idBoutonChoisi;
        }
        else{
          menu->tabBoutons[i].etat=HIGHLIGHTED;
        }
      }
    }
  }
  //Background :
  if(menu->etatanim <= 0){
    menu->spriteActuel.x += menu->spriteActuel.w;
    if(menu->spriteActuel.x >= menu->spriteActuel.w * menu->nbSprites)
      menu->spriteActuel.x=0;
    menu->etatanim = menu->animDelay;
  }
  else
    menu->etatanim --;
  return idBoutonValide;
}

/**
 * \brief Charge une sauvegarde
 * @param renderer le renderer à utiliser
 * @param numSauv la sauvegarde utilisée
 * @param perso le personnage à remplir
 * @param salle la salle à remplir
 * @return la sauvegarde chargée si réussite
*/
int chargerSauvegardeMenu(SDL_Renderer * renderer, int numSauv, personnage_t ** perso, salle_t ** salle){
  position_t delta;
  position_t blocs;
  int sauv;
  char * sallepath=malloc(sizeof(char)*TAILLEPATHFICHIER);
  sallepath[0]='\0';

  if(*perso != NULL)
    destroy_personnage(perso);
  if(*salle != NULL)
    destroy_salle(salle);
  delta.x=0;
  delta.y=TAILLEBLOC-1;
  blocs.x =1;
  blocs.y =0;
  *perso=initialisation_personnage(renderer, delta, blocs);
  sauv=chargerSauvegarde(numSauv, *perso, sallepath);
  if(sauv > 0){
    sallepath=realloc(sallepath, sizeof(char)*(strlen(sallepath)+1));
    *salle=initialiser_salle(renderer, sallepath, *perso);
    (*perso)->delta.y = TAILLEBLOC -1;
  }
  else{
    *salle=initialiser_salle(renderer, NIVEAUTXT, *perso);
    (*perso)->pos.y=(*salle)->hauteur - HAUTEURHITBOXPERS/TAILLEBLOC -2;
    (*perso)->apparition.x = (*perso)->pos.x;
    (*perso)->apparition.y = (*perso)->pos.y;
  }

  ecranNoir(renderer, 100);
  free(sallepath);
  return sauv;
}

/**
 * \brief Sauvegarde dans un slot de sauvegarde
 * @param numSauv le slot de la sauvegarde à utiliser
 * @param perso le personnage à sauvegarder
 * @param salle la salle à sauvegarder
 * @return la sauvegarde sauvegardée si réussite
*/
int sauvegarderMenu(int numSauv, personnage_t * perso, salle_t * salle){
  return sauvegarder(numSauv, perso, salle->nomFichier);
}
