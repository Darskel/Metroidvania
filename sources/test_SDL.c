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
  int mousex;
  int mousey;
  Sint16 x_move;
  Sint16 y_move;
  boolean_t fin=FALSE;
  boolean_t start=FALSE;
  audiodata_t ** audiodata = malloc(sizeof(audiodata_t *));
  *audiodata = NULL;
  //SDL_Texture * tileset=NULL;
  SDL_Texture * menu=NULL;
  int messageRes;
  SDL_MessageBoxButtonData * buttons = NULL;

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

  *audiodata = chargerWAVreplay(BEGINWAV);
  togglePauseMusic(*audiodata);

  while(!fin){

    frameStart = SDL_GetTicks();
    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_QUIT: //Appui sur la croix quitte le programme
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
          fin=menuConfirmation(renderer);
          */
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
              fin=menuConfirmation(renderer);
              */
              break;
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
            case SDLK_a:
              break;
            case SDLK_b:
              break;
            case SDLK_RETURN:
              start=TRUE;
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
          }
          break;
        case SDL_MOUSEMOTION :
          mousex=event.motion.x;
          mousey=event.motion.y;
          break;
        case SDL_MOUSEBUTTONUP:
          start=TRUE;
          break;
        case SDL_JOYBUTTONDOWN :
        switch(event.jbutton.button){
            case 0 : //bouton A manette XBOX
              break;
            case 1 : //bouton B manette XBOX
              break;
            case 7 : //bouton Start manette XBOX
              break;
          }
        break;
        case SDL_JOYBUTTONUP :
          switch(event.jbutton.button){
            case 0 : //bouton A manette XBOX
            case 7 : //bouton Start manette XBOX
              start=TRUE;
              break;
            }
          break;
        case SDL_JOYAXISMOTION :
          switch(event.jaxis.axis){
            case 0 :
              if(event.jaxis.value>ZONEMORTE){
              }
              else if(event.jaxis.value<ZONEMORTE*-1){
              }
              else{
              }
              break;
          }
          break;
        /*case SDL_JOYBALLMOTION :
          break;*/
        case SDL_JOYHATMOTION :
          switch(event.jhat.value){
            case SDL_HAT_CENTERED:
              break;
            case SDL_HAT_LEFT:
              break;
            case SDL_HAT_RIGHT:
              break;
            case SDL_HAT_UP:
              break;
            case SDL_HAT_DOWN:
              break;
          }
      }
    }
    if ( pJoystick != NULL ){
      x_move = SDL_JoystickGetAxis(pJoystick, 0);
      y_move = SDL_JoystickGetAxis(pJoystick, 1);
    }

    if(start){
      start=FALSE;
      finMusique(audiodata);
      if (jeu(fenetre, &renderer, mode, pJoystick, fullscreen, audiodata)){
        gameover(fenetre, renderer, mode, pJoystick, fullscreen, audiodata);
        *audiodata = chargerWAVreplay(BEGINWAV);
        togglePauseMusic(*audiodata);
      }
      else{
        fin = TRUE;
      }

    }

    afficher_menu(renderer);

    frameTime = SDL_GetTicks() - frameStart;
    if(frameTime < FRAMEDELAY){
      SDL_Delay(FRAMEDELAY - frameTime);
    }

  }

  if(pJoystick != NULL)
    SDL_JoystickClose(pJoystick);
  //SDL_DestroyTexture(menu);
  //SDL_DestroyTexture(tileset);
  if(*audiodata != NULL)
    finMusique(audiodata);
  free(audiodata);
  quitter_SDL(&fenetre, &renderer);
  fprintf(stdout, "Programme quitté normalement\n");
  return 0;
}
