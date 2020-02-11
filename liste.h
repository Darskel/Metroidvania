#include "structs.h"

typedef struct elemListe{
    porte_t *porte;
    struct elemListe *pred, *succ;
} elemListe;

typedef struct liste{
    elemListe *drapeau, *ec;
} liste;

liste* creerListe();
int listeVide(liste *maListe);
int horsListe(liste *maListe);
void enTete(liste *maListe);
void enQueue(liste *maListe);
void suivant(liste *maListe);
void precedent(liste *maListe);
void valeurElm(liste *maListe, porte_t *v);
void modifElm(liste *maListe, porte_t *v);
void oterElm(liste *maListe);
void ajoutDroit(liste *maListe, porte_t *v);
void ajoutGauche(liste *maListe, porte_t *v);
void supListe(liste **maListe);