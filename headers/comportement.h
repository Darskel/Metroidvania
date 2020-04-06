#ifndef COMPORTEMENT_INCLUDED
#define COMPORTEMENT_INCLUDED

/**
 * \file structs.h
 * \brief Ensemble de fonctions pour le comportement des mobs et du personnage
 * \author Thomas DIDIER L2 Info Le Mans
 * \version 1.0
 * \date 01/04/2020
*/

int hitE(monstre_t* e1, monstre_t* e2);
int hitP(monstre_t* e, personnage_t* p);

void depDroite(personnage_t* p, salle_t* s);
void depGauche(personnage_t* p, salle_t* s);
void depVert(personnage_t* p, salle_t* s, int tryJump);
char* prendPorte(personnage_t* p, liste_t* lPortes);

void compRecuperable(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites);
void compFleches(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites);
void compMurGlace(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites);
void compRoiVifplume(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites);
void compSerpent(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites);
void compSerpentRose(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites);
void compSingeGrotte(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites);
void compVersGeant(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites);
void compVifplume(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites);

#endif
