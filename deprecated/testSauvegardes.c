#include <stdlib.h>
#include <string.h>
#include "../headers/structs.h"
#include "../headers/source.h"

int main(int argc, char* argv[]){
    personnage_t p;
    salle_t* s = NULL;

    p.pv = 3;
    p.pos = (position_t){1,1};

    for(int i = 0; i < TAILLE_INVENTAIRE; i++)
        p.inventaire[i] = 1;
    
    p.nomObj[0] = "cle bleue";
    p.nomObj[1] = "cle rouge";
    p.nomObj[2] = "cle rouillee";
    p.nomObj[3] = "cle verte";
    p.nomObj[4] = "discoshroom";
    p.nomObj[5] = "double saut";
    p.nomObj[6] = "huile";
    p.nomObj[7] = "renard";
    
    lireSalle(NIVEAUTXT,&s,&p);

    printf("__Avant sauvegarde:__\n");

    printf("Pv:%d\n",p.pv);
    printf("Pos:%d,%d\n",p.pos.x,p.pos.y);
    printf("Dans:%s\n",s->nomFichier);
    printf("Inventaire:\n");
    for(int i = 0; i < TAILLE_INVENTAIRE; i++)
        printf("\t%s:%d\n",p.nomObj[i],p.inventaire[i]);

    sauvegarder(1,&p,s->nomFichier,p.pos);

    p.pv = 0;
    p.pos = (position_t){0,0};
    for(int i = 0; i < TAILLE_INVENTAIRE; i++)
        p.inventaire[i] = 0;

    printf("__Apres sauvegarde(et reinit):__\n");

    printf("Pv:%d\n",p.pv);
    printf("Pos:%d,%d\n",p.pos.x,p.pos.y);
    printf("Dans:%s\n",s->nomFichier);
    printf("Inventaire:\n");
    for(int i = 0; i < TAILLE_INVENTAIRE; i++)
        printf("\t%s:%d\n",p.nomObj[i],p.inventaire[i]);

    strcpy(s->nomFichier,"");

    chargerSauvegarde(1,&p,s->nomFichier);

    printf("__Apres chargement:__\n");

    printf("Pv:%d\n",p.pv);
    printf("Pos:%d,%d\n",p.pos.x,p.pos.y);
    printf("Dans:%s\n",s->nomFichier);
    printf("Inventaire:\n");
    for(int i = 0; i < TAILLE_INVENTAIRE; i++)
        printf("\t%s:%d\n",p.nomObj[i],p.inventaire[i]);

    nettoyerSalle(&s);

    return 0;
}