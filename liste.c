#include "liste.h"

void initListe(liste *maListe){
    maListe->drapeau = malloc(sizeof (*(maListe->drapeau)));
    maListe->ec = maListe->drapeau->pred = maListe->drapeau->succ = maListe->drapeau;
}

int listeVide(liste *maListe){
    return maListe->drapeau->succ == maListe->drapeau;
}

int horsListe(liste *maListe){
    return maListe->ec == maListe->drapeau;
}

void enTete(liste *maListe){
    if(!listeVide(maListe))
        maListe->ec = maListe->drapeau->succ;
}

void enQueue(liste *maListe){
    if(!listeVide(maListe))
        maListe->ec = maListe->drapeau->pred;
}

void suivant(liste *maListe){
    if(!horsListe(maListe))
        maListe->ec = maListe->ec->succ;
}

void precedent(liste *maListe){
    if(!horsListe(maListe))
        maListe->ec = maListe->ec->pred;
}

void valeurElm(liste *maListe, porte_t* v){
    if(!horsListe(maListe))
        *v = maListe->ec->porte;
}

void modifElm(liste *maListe, porte_t v){
    if(!horsListe(maListe))
        maListe->ec->porte = v;
}

void oterElm(liste *maListe){
    if(!horsListe(maListe)){
        maListe->ec->pred->succ = maListe->ec->succ;
        elemListe *tmp = maListe->ec->succ->pred = maListe->ec->pred;
        free(maListe->ec);
        maListe->ec = tmp;
    }
}

void ajoutDroit(liste *maListe, porte_t v){
    if(listeVide(maListe) || !horsListe(maListe)){
        elemListe *nouv = malloc(sizeof (*nouv));
        nouv->porte = v;
        nouv->succ = maListe->ec->succ;
        maListe->ec->succ = nouv;
        nouv->pred = maListe->ec;
        nouv->succ->pred = nouv;
        maListe->ec = nouv;
    }
}

void ajoutGauche(liste *maListe, porte_t v){
    if(listeVide(maListe) || !horsListe(maListe)){
        elemListe *nouv = malloc(sizeof (*nouv));
        nouv->porte = v;
        nouv->pred = maListe->ec->pred;
        maListe->ec->pred = nouv;
        nouv->succ = maListe->ec;
        nouv->pred->succ = nouv;
        maListe->ec = nouv;
    }
}

void supListe(liste *maListe){
    enQueue(maListe);
    while(!listeVide(maListe))
        oterElm(maListe);
    free(maListe->drapeau);
    free(maListe);
    maListe = NULL;
}