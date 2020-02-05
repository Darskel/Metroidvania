#include <stdlib.h>
#include <string.h>
#include "structs.h"

int nettoyerSalle(salle_t** salle){
    for(int i = 0; i < (*salle)->hauteur; i++)
        free((*salle)->mat[i]);
    
    free((*salle)->mat);
    free((*salle)->nomFichier);
    free(*salle);
    *salle = NULL;

}

int lireSalle(char* nomFichier, salle_t* salle){
    FILE * monDoc = NULL;
    char mot[50];
    int lon, larg, val;
    int cx1, cx2, cy1, cy2;

    if(salle)
        nettoyerSalle(&salle);
    
    salle = malloc(sizeof(*salle));
    salle->nomFichier = malloc(sizeof(char) * (strlen(nomFichier)+1));
    strcpy(salle->nomFichier, nomFichier);

    //creation des variables dans le tas
    monDoc = fopen(nomFichier, "r");

    //taille matrice
    fscanf(monDoc, "%d %d", &lon, &larg);

    //Création matrice
    salle->mat = malloc(sizeof(int*) * lon);
    for(int i = 0; i < lon; i++)
        salle->mat[i] = malloc(sizeof(int) * larg);

    //Remplissage matrice
    for(int i = 0; i < lon*larg; i++){
        fscanf(monDoc, "%d", &val);
        salle->mat[i/larg][i%larg] = val;
    }

    //Création et remplissage des portes
    /*fscanf(monDoc, "%s %d %d %d %d", mot, &cx1, &cy1, &cx2, &cy2);
    while(!feof(monDoc)){

        fscanf(monDoc, "%s %d %d %d %d", mot, &cx1, &cy1, &cx2, &cy2);
    }*/
    //fermeture fichier
    fclose(monDoc);
}