#ifndef SOURCE_INCLUDED
#define SOURCE_INCLUDED
#include "structs.h"

#define DIR_SAUV "sauvegardes"
#define DIR_SALLE "./salles/"

extern int sauvegarder(int, int, char*, position_t*, int[], char* []);
extern int chargerSauvegarde(int, char*, personnage_t*, int[], char*[]);
extern int lireSalle(char*, salle_t**);

#endif
