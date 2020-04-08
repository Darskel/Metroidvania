#include <stdio.h>
#include <stdlib.h>
#include "../headers/structs.h"
#include "../headers/source.h"
#include "../headers/comportement.h"

int main(int argc, char* argv[]){
    salle_t* s = NULL;
    personnage_t* p = malloc(sizeof(personnage_t));
    char c;
    int alive = 1;
    char *next = NULL;

    lireSalle("./salle_debut.txt", &s);
    p->pos = (position_t){22,10};
    p->delta = (position_t){5,0};
    p->hitbox = (taille_t){HAUTEURHITBOXPERS,LARGEURHITBOXPERS};

    for(int i = 0; i < s->hauteur; i++){
        for(int j = 0; j < s->largeur; j++)
            if(j >= p->pos.x && j < p->pos.x + 2 && i >= p->pos.y && i < p->pos.y + 4 && !s->mat[i][j])
                printf("t ");
            else
                if(s->mat[i][j])
                    printf("x ");
                else
                    printf("o ");
        printf("\n");
    }

    next = prendPorte(p,s->listePorte);
    if(next){
        alive = 0;
    }

    if(next)
        printf("Porte touchee, prochaine salle: %s\n",next);

    free(next);
    free(p);

    return 0;
}