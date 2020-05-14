#include "../headers/sdl_fonctions.h"

/**
 * \file test_SDL.c
 * \brief Fichier de test des fonctions de 'sdl_fonctions.c' (Actuellement le programme principal)
 * \author Marie-Nina MUNAR L2 Info Le Mans
 * \version 2.0
 * \date 07/04/2020
*/

/**
 * \brief Programme de test qui lance le jeu
 *
 * @param argc le nombre d'arguments du main (attendu : 0 ou 1)
 * @param argv le tableau des chaînes de caractères des arguments (attendu : rien ou "-fullscreen")
 * @return 0 si tout s'est bien déroulé
 */
int main(int argc, char *argv[]){
  boolean_t fullscreen=FALSE;

  if(argc>1){
    if(!strcmp(argv[1], "-fullscreen"))
      fullscreen=TRUE;
    else{
      fprintf(stdout, "Paramètre incorrect.\n Usage : %s -fullscreen\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  SDL_DisplayMode mode;
  SDL_Window * fenetre=NULL;
  SDL_Renderer * renderer=NULL;
  SDL_Event event;
  Uint32 frameStart;
  int frameTime;
  SDL_Point souris;
  souris.x=0;
  souris.y=0;
  int direction=0;
  int clique=0;
  boolean_t quitter=FALSE;
  boolean_t bougeSouris=FALSE;
  Sint16 x_move;
  Sint16 y_move;
  boolean_t fin=FALSE;
  boolean_t start=FALSE;
  char * question=NULL;

  int volumeAffiche=0;

  Mix_Music * musique = NULL;
  menu_t * menu=NULL;
  //SDL_Texture * tileset=NULL;
  boolean_t messageRes;

  srand(time(NULL));
  initialisation_SDL(&fenetre, &renderer, &mode, fullscreen);
  SDL_SetRenderDrawColor(renderer,0,0,0,255);
  SDL_RenderFillRect(renderer, NULL);
  //SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  SDL_Joystick* pJoystick = NULL;
  int numJoystick = SDL_NumJoysticks();
  if ( numJoystick >= 1 )
  {
      pJoystick = SDL_JoystickOpen(0);
      if ( pJoystick == NULL )
      {
          fprintf(stderr,"Erreur pour ouvrir le premier joystick\n");
      }
      else{
        printf("WARNING : Programme conçu pour manette XBOX, autres manettes non garanties\n");
        SDL_JoystickEventState(SDL_ENABLE);
      }
  }

  //tileset=initialiser_texture(TILESETPATH, renderer);
  //menu=initialiser_texture("./sprites/menu/menu.png", renderer, FALSE);
  menu = creerMenuDemarrage(renderer);
  musique = chargerMusique(BEGINWAV);
  lancerMusiqueInfini(musique);
  SDL_GetMouseState(&(souris.x), &(souris.y));
  while(!fin){

    frameStart = SDL_GetTicks();
    direction=0;
    quitter=FALSE;
    messageRes=FALSE;
    bougeSouris = FALSE;
    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_QUIT: //Appui sur la croix quitte le programme
          quitter=TRUE;
          break;
        case SDL_WINDOWEVENT:
          detruireTexturesMenu(menu);
          creerTexturesMenuDemarrage(renderer, menu);
          break;
        case SDL_KEYUP:
          switch(event.key.keysym.sym){
            case SDLK_ESCAPE://Appui sur Echap quitte le programme
              quitter=TRUE;
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
              //start=TRUE;
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
            case SDLK_KP_PLUS:
              MonterSon();
              volumeAffiche=VOLUMETIME;
              break;
            case SDLK_KP_MINUS:
              baisserSon();
              volumeAffiche=VOLUMETIME;
              break;
            case SDLK_RETURN:
              clique=1;
              //start=TRUE;
              break;
          }
          break;
        case SDL_MOUSEMOTION :
          bougeSouris = TRUE;
          break;
        case SDL_MOUSEBUTTONUP:
          clique=2;
          //start=TRUE;
          break;
        case SDL_MOUSEBUTTONDOWN:
          clique=1;
          //start=TRUE;
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
              //start=TRUE;
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
    if ( pJoystick != NULL ){
      x_move = SDL_JoystickGetAxis(pJoystick, 0);
      y_move = SDL_JoystickGetAxis(pJoystick, 1);
    }

    if(menu->idBoutonValide>menu->nbTextes)
      Mix_PlayChannel(-1, EffetsSonores[SOUND_MENU], 0);

    if(menu->idBoutonValide==2)
      start = TRUE;
    else if(menu->idBoutonValide==4)
      quitter = TRUE;

    if(start){
      start=FALSE;
      SDL_Delay(30);
      Mix_HaltMusic();
      if (jeu(fenetre, &renderer, mode, pJoystick, fullscreen)){
        gameover(fenetre, renderer, mode, pJoystick, fullscreen);
        lancerMusiqueInfini(musique);
        bougeSouris=TRUE;
      }
      else{
        fin = TRUE;
      }
    }

    if(quitter){
      messageRes=menuConfirmation(renderer, "Voulez-vous quitter ?", 3,5);
      if(messageRes)
        fin=TRUE;
    }

    afficher_menu(renderer, menu, 3, 4, TRUE);

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

  if(pJoystick != NULL)
    SDL_JoystickClose(pJoystick);
  //SDL_DestroyTexture(menu);
  //SDL_DestroyTexture(tileset);
  Mix_HaltMusic();
  detruireMenu(&menu);
  if(musique != NULL)
    Mix_FreeMusic(musique);
  quitter_SDL(&fenetre, &renderer);
  fprintf(stdout, "Programme quitté normalement\n");
  return 0;
}
