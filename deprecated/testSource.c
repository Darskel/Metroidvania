#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../headers/source.h"

int main(){
    salle_t* salle = NULL;
    personnage_t perso;
    int inventaire[TAILLE_INVENTAIRE] = {1,0,1,0,0,1};
    char* nomObj[TAILLE_INVENTAIRE] = {"arc","cle","fleche","fleche feu","pioche","zoo"};

    perso.pv = 20;

    lireSalle("1.txt",&salle);

    for(int i = 0; i < salle->hauteur; i++){
        for(int j = 0; j < salle->largeur; j++)
            printf("%d ", salle->mat[i][j]);
        printf("\n");
    }

    sauvegarder(1,perso.pv,salle->nomFichier,&((position_t){1,1}),inventaire,nomObj);

    for(int i = 0; i < salle->hauteur; i++)
        free(salle->mat[i]);
    free(salle->mat);
    supListe(&salle->listePorte,supPorte);

    strcpy(salle->nomFichier,"");
    perso.pv = 0;
    inventaire[0] = inventaire[2] = inventaire[5] = 0;

    printf("fichier: %s\npv: %d\nInv: [",salle->nomFichier, perso.pv);
    for(int i = 0; i < TAILLE_INVENTAIRE; i++)
        printf("%d, ", inventaire[i]);
    printf("]\n");

    chargerSauvegarde(1,salle->nomFichier,&perso,inventaire,nomObj);

    printf("fichier: %s\npv: %d\nInv: [",salle->nomFichier, perso.pv);
    for(int i = 0; i < TAILLE_INVENTAIRE; i++)
        printf("%d, ", inventaire[i]);
    printf("]\n");

    free(salle->nomFichier);
    free(salle);

    return 0;
}
