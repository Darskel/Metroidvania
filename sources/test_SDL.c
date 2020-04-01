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
  position_t positionDepart;
  positionDepart.x = 0;
  positionDepart.y = 10;
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
  tileset=initialiser_texture(TILESETPATH, renderer);
  salle=initialiser_salle(renderer, "salle_debut.txt", "./sprites/salles/salle_debut.png", tileset);
  perso=initialisation_personnage(renderer, positionDepart);
  afficher_salle(renderer, salle);
  SDL_Delay(500);
  void afficher_personnage(SDL_Renderer * renderer, personnage_t * perso);
  SDL_Delay(5000);
  /*destroy_salle(&salle);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  SDL_Delay(100);
  salle=initialiser_salle(renderer, "salle_entree_grotte.txt", "./sprites/salles/salle_entree_grotte.png", tileset);
  afficher_salle(renderer, salle);
  SDL_Delay(5000);*/
  destroy_salle(&salle);
  destroy_personnage(&perso);
  quitter_SDL(&fenetre, &renderer);
  renderer=NULL;
  fenetre=NULL;
  fprintf(stdout, "Programme quitté normalement\n");
  return 0;
}
