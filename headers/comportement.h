#ifndef COMPORTEMENT_INCLUDED
#define COMPORTEMENT_INCLUDED

typedef enum{
    POSEQ,
    NORD,
    NORD_EST,
    EST,
    SUD_EST,
    SUD,
    SUD_OUEST,
    OUEST,
    NORD_OUEST
} test;

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