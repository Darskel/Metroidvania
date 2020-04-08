#ifndef SOURCE_INCLUDED
#define SOURCE_INCLUDED
#include "../headers/structs.h"

#define DIR_SAUV "sauvegardes"
#define DIR_SALLE "./salles/"
#define BLOC_ERR "./autre/erreur.png"
#define BASE 100

/**
 * \file source.h
 * \brief Header et constantes des fonctions de lecture d'une salle
 * \author Thomas DIDIER L2 Info Le Mans
 * \version 1.1
 * \date 06/04/2020
*/

#define NBTYPEMONSTRE 24
type_monstre_t typesMonstre[NBTYPEMONSTRE];

char* chercherSprite(int, char*);

int sauvegarder(int, int, char*, position_t*, int[], char* []);
int chargerSauvegarde(int, char*, personnage_t*, int[], char*[]);

void creerTypeEntite();
type_monstre_t* obtenirTypesEntite();

int nettoyerSalle(salle_t** salle);
int lireSalle(char*, salle_t**);

#endif
