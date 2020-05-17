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
 * \author Thomas DIDIER & Marie-Nina MUNAR L2 Info Le Mans
 * \version 1.2
 * \date 17/05/2020
*/

#define NBTYPEMONSTRE 24 /** Nombre de type de monstres */
type_monstre_t typesMonstre[NBTYPEMONSTRE]; /** Tableau en variable globale des types de monstres*/

//char* chercherSprite(int, char*);

int sauvegarder(int numSauv, personnage_t* perso, char* salle);
int chargerSauvegarde(int numSauv, personnage_t* perso, char* salle);

void creerTypeEntite();
type_monstre_t* obtenirTypesEntite();

int nettoyerSalle(salle_t** salle);
int lireSalle(char* nomFichier, salle_t** salle, personnage_t* perso);

#endif
