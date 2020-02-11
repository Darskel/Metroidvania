#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

int testElem(liste* l, int n){
    porte_t p;
    enTete(l);
    printf("Test de mise en tete: %s\n", l->ec == l->drapeau->succ ? "OK" : "erreur");
    valeurElm(l,&p);
    if(p.spritesActuel != 2)
        return 0;
    suivant(l);
    valeurElm(l,&p);
    return p.spritesActuel == n;
}

int main(){
    liste * l = NULL;
    porte_t * p = NULL;

    printf("Etat de la liste avant creation: %p\n", l);
    l = creerListe();
    printf("Test de création de la liste: %p\n", l ? "OK": "erreur");

    if(!l)
        return 1;

    printf("Etat du drapeau apres initialisation: %p\n", l->drapeau);
    printf("Etat de l'element courant apres initialisation: %p\n", l->ec);

    printf("Test liste vide: %s\n", listeVide(l) ? "OK" : "erreur");

    p = malloc(sizeof(*p));
    p->spritesActuel = 1;
    ajoutDroit(l,p);
    p = malloc(sizeof(*p));
    p->spritesActuel = 2;
    ajoutGauche(l,p);

    printf("Test liste vide apres ajout d'elements: %s\n", listeVide(l) ? "erreur" : "OK");

    printf("Test de placement des elements: %s\n", testElem(l,1) ? "OK" : "erreur");

    enTete(l);
    suivant(l);
    p = malloc(sizeof(*p));
    p->spritesActuel = 3;
    modifElm(l,p);
    free(p);
    p = NULL;

    printf("Test de modification d'un element: %s\n", testElem(l,3) ? "OK" : "erreur");

    supListe(&l);
    printf("Etat de la liste apres suppression: %s\n", l ? "erreur" : "OK");

    return 0;

}