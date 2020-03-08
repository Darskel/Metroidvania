#include <stdio.h>
#include "../headers/sdl_fonctions.h"

int main(int argc, char* argv[]){
  int erreur;
  SDL_Window * fenetre;
  fenetre = initialisation_SDL();
  erreur=evenements(fenetre);
  if(erreur) fprintf(stdout,"Programme quitté anormalement\n");
  else fprintf(stdout, "Programme quitté normalement\n");
  return 0;
}
