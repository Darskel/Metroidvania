#ifndef INITS_INCLUDED
#define INITS_INCLUDED

#include "../headers/structs.h"

void mainInit(personnage_t** perso, liste_t** entites);
void initSdlSalle(salle_t* salle);
void freeSdlSalle(salle_t* salle);
void initCreerPartie(personnage_t* perso, salle_t* salle);
void quitterPartie(salle_t* salle);

#endif