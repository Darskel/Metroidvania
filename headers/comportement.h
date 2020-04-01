#ifndef COMPORTEMENT_INCLUDED
#define COMPORTEMENT_INCLUDED

int hitE(monstre_t* e1, monstre_t* e2);
int hitP(monstre_t* e, personnage_t* p);

void depDroite(personnage_t* p, salle_t* s);
void depGauche(personnage_t* p, salle_t* s);

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