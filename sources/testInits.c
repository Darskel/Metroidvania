#include <stdio.h>
#include <stdlib.h>
#include "../headers/structs.h"
#include "../headers/source.h"
#include "../headers/inits.h"

int main(){
    salle_t* s = NULL;
    personnage_t* p = NULL;
    liste_t* e = NULL;

    mainInit(&p,&s,&e);
    initCreerPartie(p,s);
    quitterPartie(s);

    //liberation salle
    for(int i = 0; i < s->hauteur; i++)
        free(s->mat[i]);
    free(s->mat);
    supListe(&s->listePorte,supPorte);

    free(s->nomFichier);
    free(s);

    //liberation entités
    supListe(&e,supMonstre);

    //liberation perso
    //unload sprites

    return 0;
}