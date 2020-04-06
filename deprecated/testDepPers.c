#include <stdio.h>
#include "../headers/structs.h"
#include "../headers/source.h"
#include "../headers/comportement.h"

int main(int argc, char* argv[]){
    salle_t* s = NULL;
    personnage_t* p = malloc(sizeof(personnage_t));

    lireSalle("./salle_debut.txt", &s);
    p->pos = (position_t){0,10};
    p->delta = (position_t){0,0};
    p->hitbox = (taille_t){4*8,2*8};

    for(int i = 0; i < s->hauteur; i++){
        for(int j = 0; j < s->largeur; j++)
            if(j >= p->pos.x && j < p->pos.x + p->hitbox.largeur/8 && i >= p->pos.y && i < p->pos.y + p->hitbox.hauteur/8 && !s->mat[i][j])
                printf("t ");
            else
                if(s->mat[i][j])
                    printf("x ");
                else
                    printf("o ");
        printf("\n");
    }

    printf("pos: %s\n", persValidDep(p,s) ? "Valides" : "Non valides");

    free(p);

    return 0;
}