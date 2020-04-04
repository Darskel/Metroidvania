#ifndef SOURCE_INCLUDED
#define SOURCE_INCLUDED
#include "../headers/structs.h"

#define DIR_SAUV "sauvegardes"
#define DIR_SALLE "./salles/"
#define BLOC_ERR "./autre/erreur.png"
#define BASE 100

extern char* chercherSprite(int, char*);
extern int sauvegarder(int, int, char*, position_t*, int[], char* []);
extern int chargerSauvegarde(int, char*, personnage_t*, int[], char*[]);
int nettoyerSalle(salle_t** salle);
extern int lireSalle(char*, salle_t**);

#endif