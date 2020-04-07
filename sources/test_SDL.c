#include "../headers/sdl_fonctions.h"

/**
 * \file test_SDL.c
 * \brief Fichier de test des fonctions de 'sdl_fonctions.c' (Actuellement le programme principal)
 * \author Marie-Nina MUNAR L2 Info Le Mans
 * \version 1.1
 * \date 06/04/2020
*/

/**
 * \brief Programme de test qui initalise la sdl
 *
 * @param argc le nombre d'arguments du main (attendu : 0 ou 1)
 * @param argv le tableau des chaînes de caractères des arguments (attendu : rien ou "-fullscreen")
 * @return 0 si tout s'est bien déroulé
 */
int main(int argc, char *argv[]){
  boolean_t fullscreen=FALSE;
  creerTypeEntite(); //ajouté par thomas
  SDL_DisplayMode mode;
  SDL_Window * fenetre=NULL;
  SDL_Renderer * renderer=NULL;
  if(argc>1){
    if(!strcmp(argv[1], "-fullscreen"))
      fullscreen=TRUE;
    else{
      fprintf(stdout, "Paramètre incorrect.\n Usage : %s -fullscreen\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }
  initialisation_SDL(&fenetre, &renderer, &mode, fullscreen);
  SDL_SetRenderDrawColor(renderer,0,0,0,255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  evenements(renderer, &mode);
  quitter_SDL(&fenetre, &renderer);
  renderer=NULL;
  fenetre=NULL;
  fprintf(stdout, "Programme quitté normalement\n");
  return 0;
}
