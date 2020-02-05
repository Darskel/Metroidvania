#include <stdlib.h>
#include <string.h>
#include "structs.h"

int nettoyerSalle(salle_t** salle){
    for(int i = 0; i < (*salle)->hauteur; i++){
        for(int j = 0; j < (*salle)->largeur; j++)
            free((*salle)->mat[i][j]);
        free((*salle)->mat[i]);
    }
    free((*salle)->mat);
    free((*salle)->nomFichier);
    free(*salle);
    *salle = NULL;

}

int lireSalle(char* nomFichier, salle_t* salle){
    if(salle)
        nettoyerSalle(&salle);
    
    salle = malloc(sizeof(*salle));
    salle->nomFichier = malloc(sizeof(char) * (strlen(nomFichier)+1));
    strcpy(salle->nomFichier, nomFichier);

    //creation des variables dans le tas
    //lecture fichier et ajustement des variables
    //fermeture fichier
}