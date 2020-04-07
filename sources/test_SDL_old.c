#include <stdio.h>
#include "../headers/sdl_fonctions.h"

/**
 * \file test_SDL.c
 * \brief Fichier de test des fonctions de 'sdl_fonctions.c'
 * \author Marie-Nina MUNAR L2 Info Le Mans
 * \version 1.0
 * \date 25/02/2020
*/

/**
 * \brief Programme de test qui initalise la sdl, lance la gestion d'évènements et gère les erreurs
 *
 * @return 0 si tout s'est bien déroulé
 */
int main(int argc, char *argv[]){
  int erreur;
  SDL_Window * fenetre;
  fenetre = initialisation_SDL();
  erreur=evenements(fenetre);
  if(erreur) fprintf(stdout,"Programme quitté anormalement\n");
  else fprintf(stdout, "Programme quitté normalement\n");
  return 0;
}
