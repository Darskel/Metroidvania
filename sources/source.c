#include <stdlib.h>
#include <string.h>
#include "../headers/structs.h"

/**
 * \file source.c
 * \brief Fonctions de lecture d'une salle
 * \author Thomas DIDIER L2 Info Le Mans
 * \version 1.0
 * \date 13/02/2020
*/

/**
 * \brief Nettoie et supprime la structure salle donnée
 * \details libère toute la mémoire utiliser et prépare le pointeur à être de nouveau utilisé
 * 
 * @param salle structure salle à traiter
*/
int nettoyerSalle(salle_t** salle){
    for(int i = 0; i < (*salle)->hauteur; i++)
        free((*salle)->mat[i]);

    supListe(&((*salle)->listePorte));
    free((*salle)->mat);
    free((*salle)->nomFichier);
    free(*salle);
    *salle = NULL;

}

/**
 * \brief Modifie la structure salle pour la nouvelle salle
 * 
 * @param nomFichier nom du fichier de la salle à lire
 * @param salle structure salle du programme
*/
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

    salle->listePorte = creerListe("porte");
    porte_t* p;

    //Création et remplissage des portes
    fscanf(monDoc, "%s %d %d %d %d", mot, &cx1, &cy1, &cx2, &cy2);
    while(!feof(monDoc)){
        p = malloc(sizeof(*p));
        p->pos.x = cx1;
        p->pos.y = cy1;
        p->pos_arrivee.x = cx2;
        p->pos_arrivee.y = cy2;
        p->salleSuivante = malloc(sizeof(char) * (strlen(mot) + 1));
        strcpy(p->salleSuivante, mot);
        //Gestion des sprites de portes potentiellement à modifier
        p->spritesActuel = -1;
        p->listeSprites = NULL;
        ajoutDroit(salle->listePorte, p);
        fscanf(monDoc, "%s %d %d %d %d", mot, &cx1, &cy1, &cx2, &cy2);
    }
    //fermeture fichier
    fclose(monDoc);
}