#include "../headers/sdl_fonctions.h"

/**
 * \file test_SDL.c
 * \brief Fichier de test des fonctions de 'sdl_fonctions.c'
 * \author Marie-Nina MUNAR L2 Info Le Mans
 * \version 1.0
 * \date 25/02/2020
*/

/**
 * \brief Programme de test qui initalise la sdl
 *
 * @return 0 si tout s'est bien déroulé
 */
int main(int argc, char *argv[]){
  boolean_t fullscreen=FALSE;
  SDL_DisplayMode mode;
  SDL_Window * fenetre=NULL;
  SDL_Renderer * renderer=NULL;
  SDL_Texture * tileset;
  salle_t * salle;
  personnage_t * perso;
  if(argc>1){
    if(!strcmp(argv[1], "-fullscreen"))
      fullscreen=TRUE;
    else{
      fprintf(stdout, "Paramètre incorrect.\n Usage : %s -fullscreen\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }
  initialisation_SDL(&fenetre, &renderer, &mode, fullscreen);
  SDL_SetRenderDrawColor(renderer,20,0,0,255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  tileset=initialiser_texture(TILESETPATH, renderer, SDL_TEXTUREACCESS_STATIC);
  salle=initialiser_salle(renderer, "salle_entree_grotte.txt", tileset);
  perso=initialisation_personnage(renderer);
  evenements(fenetre,renderer,&mode);
  nettoyerSalle(&salle);
  quitter_SDL(&fenetre, &renderer);
  renderer=NULL;
  fenetre=NULL;
  fprintf(stdout, "Programme quitté normalement\n");
  return 0;
}
