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
  char * salle_nom = NULL;
  boolean_t Gauche = FALSE;
  boolean_t Droite = FALSE;
  boolean_t tryJump = FALSE;
  boolean_t tryAtk = FALSE;
  boolean_t fin=FALSE;
  boolean_t salleChangee=FALSE;
  SDL_Texture * tileset=NULL;
  salle_t * salle=NULL;
  personnage_t * perso=NULL;
  position_t positionDepart;
  position_t positionDepartDelta;
  boolean_t kon = FALSE;
  char konami[TAILLEKONAMI];
  int indKon = 0;
  for(int i = 0; i < TAILLEKONAMI; i++)
    konami[i] = '\0';

  initialisation_SDL(&fenetre, &renderer, &mode, fullscreen);
  SDL_SetRenderDrawColor(renderer,0,0,0,255);
  SDL_RenderClear(renderer);
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



  initialiser_typeentites(renderer);
  tileset=initialiser_texture(TILESETPATH, renderer);
  salle=initialiser_salle(renderer, NIVEAUTXT, tileset);

  positionDepart.x = 1;
  positionDepartDelta.x = 0;
  positionDepart.y = salle->hauteur - HAUTEURHITBOXPERS/TAILLEBLOC -2;
  positionDepartDelta.y = TAILLEBLOC-1;

  perso=initialisation_personnage(renderer, positionDepart, positionDepartDelta);

  while(!fin){

    frameStart = SDL_GetTicks();
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
            case 7 : //bouton Start manette XBOX
              konami[indKon++] = 's';
              break;
          }
        break;
        /*case SDL_JOYBUTTONUP :
          break;*/
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

      salle_nom=prendPorte(perso, salle->listePorte);
      if(salle_nom != NULL){
        destroy_salle(&salle);
        salle=initialiser_salle(renderer, salle_nom, tileset);
        salleChangee=TRUE;
        kon = FALSE;
        free(salle_nom);
        salle_nom=NULL;
      }

      depVert(perso, salle, tryJump);


      if(Gauche){
        depGauche(perso, salle);
        if(!Droite)
          perso->direction = LEFT;
      }

      if(Droite){
        depDroite(perso, salle);
        if(!Gauche)
          perso->direction = RIGHT;
      }

      if((!Gauche&&!Droite) || (Gauche&&Droite))
        if(perso->etat == RUNNING)//ajouter par Thomas: on souhaite passer de RUNNING à IDLE mais pas de JUMPING à IDLE ou bien de ATTACKING à IDLE
          perso->etat=IDLE;

      tryJump=FALSE;

      attaquer(perso,salle,tryAtk);

      tryAtk=FALSE;

      evolution(perso,salle);

      miseAjourSprites(perso);
      miseAjourSpritesEntites(salle);

      if(salleChangee){
        ecranNoir(renderer,150);
        salleChangee=FALSE;
      }

      affichage_complet(renderer, salle, perso);

      frameTime = SDL_GetTicks() - frameStart;
      if(frameTime < FRAMEDELAY){
        SDL_Delay(FRAMEDELAY - frameTime);
      }
    }
    destroy_salle(&salle);
    destroy_personnage(&perso);
    destroy_typeentites();
    if(pJoystick != NULL)
      SDL_JoystickClose(pJoystick);
    quitter_SDL(&fenetre, &renderer);
  fprintf(stdout, "Programme quitté normalement\n");
  return 0;
}
