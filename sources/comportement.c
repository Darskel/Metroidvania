#include <stdlib.h>
#include <string.h>
#include "../headers/structs.h"
#include "../headers/liste.h"
#include "../headers/comportement.h"

static test cmpPos(position_t* p1, position_t* p2){
    if(p1->x == p2->x && p1->y == p2->y)
        return POSEQ;
    if(p1->x > p2->x){
        if(p1->y == p2->y)
            return SUD;
        if(p1->y > p2->y)
            return SUD_EST;
        return SUD_OUEST;
    }else{
        if(p1->y == p2->y)
            return NORD;
        if(p1->y > p2->y)
            return NORD_EST;
        return NORD_OUEST;
    }
    return p1->y > p2->y ? EST : OUEST;
}

static void recupElem(monstre_t* entite, personnage_t* perso){
    for(int i = 0; i < TAILLE_INVENTAIRE; i++){
        if(!strcmp(entite->type->nom,perso->nomObj[i])){
            perso->inventaire[i] = 1;
            entite->pv = 0;
        }
    }
}

static int hitP(monstre_t* entite, personnage_t* perso){
    switch(cmpPos(&(entite->pos),&(perso->pos))){
        case POSEQ: return 1;
        case NORD:
            if(((float) perso->delta.delta_x.numerateur)/perso->delta.delta_x.denominateur < -(1.0/entite->type->hauteur))
                return 1;
        case EST:
            if(((float) perso->delta.delta_y.numerateur)/perso->delta.delta_y.denominateur < -(1.0/entite->type->largeur))
                return 1;
        case SUD:
            if(((float) perso->delta.delta_x.numerateur)/perso->delta.delta_x.denominateur > 1.0/entite->type->hauteur)
                return 1;
        case OUEST:
            if(((float) perso->delta.delta_y.numerateur)/perso->delta.delta_y.denominateur > 1.0/entite->type->largeur)
                return 1;
        case NORD_EST:
            if(((float) perso->delta.delta_x.numerateur)/perso->delta.delta_x.denominateur < -(1.0/entite->type->hauteur))
                if(((float) perso->delta.delta_y.numerateur)/perso->delta.delta_y.denominateur < -(1.0/entite->type->largeur))
                    return 1;
        case SUD_EST:
            if(((float) perso->delta.delta_x.numerateur)/perso->delta.delta_x.denominateur > 1.0/entite->type->hauteur)
                if(((float) perso->delta.delta_y.numerateur)/perso->delta.delta_y.denominateur < -(1.0/entite->type->largeur))
                    return 1;
        case SUD_OUEST:
            if(((float) perso->delta.delta_x.numerateur)/perso->delta.delta_x.denominateur > 1.0/entite->type->hauteur)
                if(((float) perso->delta.delta_y.numerateur)/perso->delta.delta_y.denominateur > 1.0/entite->type->largeur)
                    return 1;
        case NORD_OUEST:
            if(((float) perso->delta.delta_x.numerateur)/perso->delta.delta_x.denominateur < -(1.0/entite->type->hauteur))
                if(((float) perso->delta.delta_y.numerateur)/perso->delta.delta_y.denominateur > 1.0/entite->type->largeur)
                    return 1;
        default:
            return 0;
    }
}

static int hitE(monstre_t* entite, monstre_t* ent2){
    switch(cmpPos(&(entite->pos),&(ent2->pos))){
        case POSEQ: return 1;
        case NORD:
            if(((float) ent2->delta.delta_x.numerateur)/ent2->delta.delta_x.denominateur < -(1.0/entite->type->hauteur))
                return 1;
        case EST:
            if(((float) ent2->delta.delta_y.numerateur)/ent2->delta.delta_y.denominateur < -(1.0/entite->type->largeur))
                return 1;
        case SUD:
            if(((float) ent2->delta.delta_x.numerateur)/ent2->delta.delta_x.denominateur > 1.0/entite->type->hauteur)
                return 1;
        case OUEST:
            if(((float) ent2->delta.delta_y.numerateur)/ent2->delta.delta_y.denominateur > 1.0/entite->type->largeur)
                return 1;
        case NORD_EST:
            if(((float) ent2->delta.delta_x.numerateur)/ent2->delta.delta_x.denominateur < -(1.0/entite->type->hauteur))
                if(((float) ent2->delta.delta_y.numerateur)/ent2->delta.delta_y.denominateur < -(1.0/entite->type->largeur))
                    return 1;
        case SUD_EST:
            if(((float) ent2->delta.delta_x.numerateur)/ent2->delta.delta_x.denominateur > 1.0/entite->type->hauteur)
                if(((float) ent2->delta.delta_y.numerateur)/ent2->delta.delta_y.denominateur < -(1.0/entite->type->largeur))
                    return 1;
        case SUD_OUEST:
            if(((float) ent2->delta.delta_x.numerateur)/ent2->delta.delta_x.denominateur > 1.0/entite->type->hauteur)
                if(((float) ent2->delta.delta_y.numerateur)/ent2->delta.delta_y.denominateur > 1.0/entite->type->largeur)
                    return 1;
        case NORD_OUEST:
            if(((float) ent2->delta.delta_x.numerateur)/ent2->delta.delta_x.denominateur < -(1.0/entite->type->hauteur))
                if(((float) ent2->delta.delta_y.numerateur)/ent2->delta.delta_y.denominateur > 1.0/entite->type->largeur)
                    return 1;
        default:
            return 0;
    }
}

static int hitB(monstre_t* entite, salle_t* salle){
    for(int i = 0; i < salle->hauteur; i++){
        for(int j = 0; j < salle->hauteur; j++){
            switch(cmpPos(&(entite->pos),&((position_t){i,j}))){
                case POSEQ: return 1;
                    break;
                case NORD:
                    if(((float) entite->delta.delta_x.numerateur)/entite->delta.delta_x.denominateur < 0)
                        return 1;
                    break;
                case EST:
                    if(((float) entite->delta.delta_y.numerateur)/entite->delta.delta_y.denominateur < 0)
                        return 1;
                    break;
                case SUD:
                    if(((float) entite->delta.delta_x.numerateur)/entite->delta.delta_x.denominateur > 0)
                        return 1;
                    break;
                case OUEST:
                    if(((float) entite->delta.delta_y.numerateur)/entite->delta.delta_y.denominateur > 0)
                        return 1;
                    break;
                case NORD_EST:
                    if(((float) entite->delta.delta_x.numerateur)/entite->delta.delta_x.denominateur < 0)
                        if(((float) entite->delta.delta_y.numerateur)/entite->delta.delta_y.denominateur < 0)
                            return 1;
                    break;
                case SUD_EST:
                    if(((float) entite->delta.delta_x.numerateur)/entite->delta.delta_x.denominateur > 0)
                        if(((float) entite->delta.delta_y.numerateur)/entite->delta.delta_y.denominateur < 0)
                            return 1;
                    break;
                case SUD_OUEST:
                    if(((float) entite->delta.delta_x.numerateur)/entite->delta.delta_x.denominateur > 0)
                        if(((float) entite->delta.delta_y.numerateur)/entite->delta.delta_y.denominateur > 0)
                            return 1;
                    break;
                case NORD_OUEST:
                    if(((float) entite->delta.delta_x.numerateur)/entite->delta.delta_x.denominateur < 0)
                        if(((float) entite->delta.delta_y.numerateur)/entite->delta.delta_y.denominateur > 0)
                            return 1;
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}

static void dep(monstre_t* entite){
    if(entite->pv)
        if(entite->direction){
            entite->delta.delta_x.numerateur += entite->type->vit_dep;
            if(entite->delta.delta_x.numerateur == entite->delta.delta_x.denominateur){
                (entite->pos.x)++;
                entite->delta.delta_x.numerateur = 0;
            }
        }else{
            entite->delta.delta_x.numerateur -= entite->type->vit_dep;
            if(entite->delta.delta_x.numerateur == -entite->delta.delta_x.denominateur){
                (entite->pos.x)--;
                entite->delta.delta_x.numerateur = 0;
            }
        }
}

static inRange(monstre_t* entite, personnage_t* perso, int radius){
    //verif si il n'y a pas de bloc bloquant entre
    int delta = perso->pos.x - entite->pos.x;

    if(delta < radius && delta > -radius)
        return 1;
    if(delta > radius || delta < -radius)
        return 0;

    int deltaF = perso->delta.delta_x.numerateur - entite->delta.delta_x.numerateur;

    if((delta > 0 && deltaF < 0) || (delta < 0 && deltaF > 0))
        return 1;
    return 0;
}

void compRecuperable(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
    if(hitP(entite,perso))
        recupElem(entite,perso);

    //gestion des sprites
    entite->spritesActuel = (entite->spritesActuel + 1)%(entite->type->nb_sprites);
}

void compFleches(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
    if(strcmp(lEntites->type, "monstre"))
        return;
    monstre_t* tmp = malloc(sizeof(monstre_t));
    enTete(lEntites);
    while(!horsListe(lEntites)){
        valeurElm(lEntites,tmp);
        if(hitE(entite,tmp) && entite->pv){
            tmp->pv -= entite->type->degat;
            modifElm(lEntites,tmp);
            entite->pv = 0;
        }
        suivant(lEntites);
    }
    free(tmp);

    if(hitB(entite,salle))
        entite->pv = 0;
    
    //gestion deb
    dep(entite);

    //gestion des sprites
}

void compMurGlace(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
    if(entite->pv)
        entite->pv = 2;
    /*ou
    if(entite->pv >= 10)
        entite->pv = 11;
    else
        (entite->pv)--;
    */
}

void compRoiVifplume(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
    //fonction en beta
    if(hitP(entite,perso)){
        perso->pv -= entite->type->degat;
    }
    if(!hitB(entite,salle)){
        dep(entite);
    }
    //gestion sprite
}

void compSerpent(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
    if(hitP(entite,perso)){
        perso->pv -= entite->type->degat;
        entite->direction = 1 - entite->direction;
        //sprite
    }else{
        if(hitB(entite,salle)){
            entite->direction = 1 - entite->direction;
            //sprite
        }
    }
    //gestion sprite
}

void compSerpentRose(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
    if(hitP(entite,perso)){
        perso->pv -= entite->type->degat;
        entite->direction = 1 - entite->direction;
        //sprite
    }else{
        if(hitB(entite,salle)){
            entite->direction = 1 - entite->direction;
            //sprite
        }else{
            if(inRange(entite,perso,6)){
                //attaquer
            }
        }
    }
    //gestion sprite
}

void compSingeGrotte(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
    if(inRange(entite,perso,4)){
        //attaquer
    }
}

void compVersGeant(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){

}

void compVifplume(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){

}