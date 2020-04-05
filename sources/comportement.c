#include <stdlib.h>
#include <string.h>
//#include <SDL2/SDL.h>
#include "../headers/structs.h"
#include "../headers/liste.h"
#include "../headers/comportement.h"

/*static int toucher(monstre_t* e1, monstre_t* e2){
    SDL_rect rect1, rect2;

    rect1.x = TAILLE_BLOCK*e1->pos.x + e1->delta.x;
    rect1.y = TAILLE_BLOCK*e1->pos.y + e1->delta.y;
    rect1.w = e1->type->hitbox.largeur;
    rect1.h = e1->type->hitbox.hauteur;

    rect2.x = TAILLE_BLOCK*e2->pos.x + e2->delta.x;
    rect2.y = TAILLE_BLOCK*e2->pos.y + e2->delta.y;
    rect2.w = e2->type->hitbox.largeur;
    rect2.h = e2->type->hitbox.hauteur;

    return SDL_IntersectRect(&rect1,&rect2);
}*/

/**
 * \brief Recupère l'élément récupérable (type monstre_t)
 *
 * @param entite pointeur vers l'élément à récupérer
 * @param perso pointeur vers le personnage pour lui crediter l'objet
*/
static void recupElem(monstre_t* entite, personnage_t* perso){
    for(int i = 0; i < TAILLE_INVENTAIRE; i++){
        if(!strcmp(entite->type->nom,perso->nomObj[i])){
            perso->inventaire[i] = 1;
            entite->pv = 0;
        }
    }
}

/**
 * \brief Vérifie si une entité touche une autre entité
 *
 * @param e1 pointeur vers la première entité
 * @param e2 pointeur vers la seconde entité
 *
 * @return 1 (TRUE) si il y a contact, 0 (FALSE) sinon
*/
int hitE(monstre_t* e1, monstre_t* e2){
    int leftE1, leftE2;
    int rightE1, rightE2;
    int topE1, topE2;
    int bottomE1, bottomE2;

    leftE1 = e1->pos.x*TAILLEBLOC + e1->delta.x;
    topE1 = e1->pos.y*TAILLEBLOC + e1->delta.y;
    rightE1 = leftE1 + e1->type->hitbox.largeur;
    bottomE1 = topE1 + e1->type->hitbox.hauteur;

    leftE2 = e2->pos.x*TAILLEBLOC + e2->delta.x;
    topE2 = e2->pos.y*TAILLEBLOC + e2->delta.y;
    rightE2 = leftE2 + e2->type->hitbox.largeur;
    bottomE2 = topE2 + e2->type->hitbox.hauteur;

    if(bottomE1 <= topE2)
        return FALSE;

    if(topE1 >= bottomE2)
        return FALSE;

    if(rightE1 <= leftE2)
        return FALSE;

    if(leftE1 >= rightE2)
        return FALSE;

    return TRUE;
}

/**
 * \brief Vérifie si une entité touche une autre entité
 *
 * @param e1 pointeur vers la première entité
 * @param e2 pointeur vers la seconde entité
 *
 * @return 1 (TRUE) si il y a contact, 0 (FALSE) sinon
*/
int hitP(monstre_t* e, personnage_t* p){
    int leftE, leftP;
    int rightE, rightP;
    int topE, topP;
    int bottomE, bottomP;

    leftE = e->pos.x*TAILLEBLOC + e->delta.x;
    topE = e->pos.y*TAILLEBLOC + e->delta.y;
    rightE = leftE + e->type->hitbox.largeur;
    bottomE = topE + e->type->hitbox.hauteur;

    leftP = p->pos.x*TAILLEBLOC + p->delta.x;
    topP = p->pos.y*TAILLEBLOC + p->delta.y;
    rightP = leftP + p->hitbox.largeur;
    bottomP = topP + p->hitbox.hauteur;

    if(bottomE <= topP)
        return FALSE;

    if(topE >= bottomP)
        return FALSE;

    if(rightE <= leftP)
        return FALSE;

    if(leftE >= rightP)
        return FALSE;

    return TRUE;
}

/**
 * \brief Vérifie si une entité est dans un bloc que la salle
 *
 * @param e pointeur vers l'entité
 * @param s pointeur vers la salle
 *
 * @return 1 (TRUE) si il y a contact, 0 (FALSE) sinon
*/
static int hitB(monstre_t* e, salle_t* s){
    int leftE;
    int rightE;
    int topE;
    int bottomE;

    leftE = e->pos.x;
    rightE = e->pos.x + e->delta.x ? 1 : 0;
    topE = e->pos.y;
    bottomE = e->pos.y + e->delta.y ? 1 : 0;

    if(leftE < 0 || topE < 0)
        return FALSE;

    for(int i = leftE; i <= rightE; i++)
        for(int j = topE; j <= bottomE; j++)
            if(j >= s->largeur || i >= s->hauteur || s->mat[i][j])
                return TRUE;

    return FALSE;
}

/**
 * \brief Vérifie si le deplacement du personnage est valide
 *
 * @param p pointeur vers le personnage
 * @param s pointeur vers la salle
 *
 * @return 1 (TRUE) si le déplacement est valide, 0 (FALSE) sinon
*/
int persValidDep(personnage_t* p, salle_t* s){
    int leftP;
    int rightP;
    int topP;
    int bottomP;

    leftP = p->pos.x*TAILLEBLOC + p->delta.x + OFFSETHITBOX;
    rightP = leftP + p->hitbox.largeur;
    topP = p->pos.y*TAILLEBLOC + p->delta.y + 1;
    bottomP = topP + p->hitbox.hauteur;

    if(leftP < 0 || topP < 0)
        return FALSE;

    leftP /= 8;
    rightP = rightP/8 + (rightP%8 ? 1 : 0) - 1;
    topP /= 8;
    bottomP = bottomP/8 + (bottomP%8 ? 1 : 0) - 1;

    //printf("_l%d_r%d_t%d_b%d_\n",leftP,rightP,topP,bottomP);

    if(bottomP >= s->hauteur || rightP >= s->largeur)
        return FALSE;

    //printf("_l%d_r%d_t%d_b%d_\n",leftP,rightP,topP,bottomP);

    for(int i = leftP; i <= rightP; i++)
        for(int j = topP; j <= bottomP; j++)
            if(s->mat[j][i]){
                //printf("_%d,%d:%d_\n",j,i,s->mat[j][i]);
                return FALSE;
            }

    return TRUE;
}

/**
 * \brief Gère le déplacement vers la droite du personnage quelque soit son état
 *
 * @param p pointeur vers le personnage
 * @param s pointeur vers la salle
*/
void depDroite(personnage_t* p, salle_t* s){
    switch(p->etat){
        case IDLE:
        case RUNNING:
        case JUMPING:
        case FALLING:
            p->delta.x += p->vit_dep;
            if(p->delta.x >= TAILLEBLOC){
                (p->pos.x)++;
                //p->delta.x = 0;
                p->delta.x -= TAILLEBLOC;
            }
            //Ajouté par MN :
            p->posxhitbox += p->vit_dep;
            //
            if(persValidDep(p,s)){
              //Modifié par MN :
              if(p->etat==IDLE){
                p->etat = RUNNING;
                p->newEtat = TRUE;
              }
              //
            }
            else{
                p->delta.x = TAILLEBLOC - 1 + OFFSETHITBOX;
                if(p->delta.x >= TAILLEBLOC)
                    p->delta.x %= TAILLEBLOC;
                else
                    (p->pos.x)--;
                //Ajouté par MN :
                p->posxhitbox -= p->vit_dep;
                //
            }
            break;
        default:
            break;
    }
}

/**
 * \brief Gère le déplacement vers la gauche du personnage quelque soit son état
 *
 * @param p pointeur vers le personnage
 * @param s pointeur vers la salle
*/
void depGauche(personnage_t* p, salle_t* s){
    switch(p->etat){
        case IDLE:
        case RUNNING:
        case JUMPING:
        case FALLING:
            p->delta.x -= p->vit_dep;
            if(p->delta.x < 0){
                (p->pos.x)--;
                //p->delta.x = TAILLEBLOC -1;
                p->delta.x += TAILLEBLOC;
            }
            //Ajouté par MN :
            p->posxhitbox -= p->vit_dep;
            //
            if(persValidDep(p,s)){
              //Modifié par MN :
              if(p->etat==IDLE){
                p->etat = RUNNING;
                p->newEtat = TRUE;
              }
              //
            }
            else{
                p->delta.x = TAILLEBLOC - OFFSETHITBOX;
                //Ajouté par MN :
                p->posxhitbox += p->vit_dep;
                //
            }
            break;
        default:
            break;
    }
}

int verifCaseUp(personnage_t* p, salle_t* s){
    int leftP;
    int rightP;
    int topP;

    leftP = p->pos.x*TAILLEBLOC + p->delta.x + OFFSETHITBOX;
    rightP = leftP + p->hitbox.largeur;
    topP = p->pos.y*TAILLEBLOC + p->delta.y;

    if(topP < 0)
        return TRUE;

    leftP /= 8;
    rightP = rightP/8 + (rightP%8 ? 1 : 0) - 1;
    topP /= 8;

    for(int i = leftP; i <= rightP; i++)
        if(s->mat[topP][i])
            return TRUE;

    return FALSE;
}

int verifCaseDown(personnage_t* p, salle_t* s){
    int leftP;
    int rightP;
    int bottomP;

    leftP = p->pos.x*TAILLEBLOC + p->delta.x + OFFSETHITBOX;
    rightP = leftP + p->hitbox.largeur;
    bottomP = p->pos.y*TAILLEBLOC + p->delta.y + 1;
    bottomP += p->hitbox.hauteur + 1;

    leftP /= 8;
    rightP = rightP/8 + (rightP%8 ? 1 : 0) - 1;
    bottomP = bottomP/8 + (bottomP%8 ? 1 : 0) - 1;

    if(bottomP >= s->hauteur )
        return -1; //Le personnage est tombé dans un trou

    for(int i = leftP; i <= rightP; i++)
        if(s->mat[bottomP][i])
            return TRUE;

    return FALSE;
}

void depVert(personnage_t* p, salle_t* s, int tryJump){
    switch(p->etat){
        case IDLE:
        case RUNNING:
            if(tryJump){
                p->etat = JUMPING;
                p->newEtat = 1;
                p->nbSaut = 1;
                p->nbPxSaut = 0;
            }else
                if(verifCaseUp(p,s)){
                    p->etat = FALLING;
                    p->newEtat = 1;
                }
            break;
        case JUMPING:
            if(verifCaseUp(p,s)){
                p->etat = FALLING;
                p->newEtat = 1;
            }
            else
                if(tryJump && p->nbSaut < 1 + p->inventaire[6]){
                    (p->nbSaut)++;
                    p->nbPxSaut = 0;
                }
                else
                    if(p->nbPxSaut >= 4*TAILLEBLOC){
                        p->etat = FALLING;
                        p->newEtat = 1;
                    }else{
                        //continuer saut
                        p->delta.y -= p->vit_dep;
                        if(p->delta.y < 0){
                            (p->pos.y)--;
                            p->delta.y += TAILLEBLOC;
                        }
                        if(!persValidDep(p,s)){
                            //traitement si le pixel juste au dessus est accessible mais pas la position d'arrivé
                            //aka trouver le position de cognement
                            (p->pos.y)++;
                            p->delta.y = 1;
                        }
                    }
            break;
        case FALLING:
            if(verifCaseDown(s,p)){
                p->etat = IDLE; //défini comme tel pour éviter de sortir de etat_t mais on ne sait pas si il est IDLE ou RUNNING
                p->nbSaut = 0;
                p->newEtat = 1;
            }else
                if(tryJump && p->nbSaut < 1 + p->inventaire[6]){
                    (p->nbSaut)++;
                    p->nbPxSaut = 0;
                }
                else{
                    //continuer chute
                    p->delta.y += p->vit_dep;
                    if(p->delta.y >= TAILLEBLOC){
                        (p->pos.y)++;
                        p->delta.y -= TAILLEBLOC;
                    }
                    if(!persValidDep(p,s)){
                        //traitement si le pixel juste en dessous est accessible mais pas la position d'arrivé
                        //aka trouver le position de cognement
                        (p->pos.y)--;
                        p->delta.y = 1;
                    }
                }
            break;
        default:
            break;
    }
}

static void dep(monstre_t* entite, salle_t* salle){
    if(entite->pv){
        if(entite->direction){
            entite->delta.x += entite->type->vit_dep;
            if(entite->delta.x >= TAILLEBLOC){
                (entite->pos.x)++;
                entite->delta.x = 0;
            }
            if(hitB(entite,salle)){
                entite->direction = LEFT;
                (entite->pos.x)--;
                entite->delta.x = TAILLEBLOC - 1;
            }
        }else{
            entite->delta.x -= entite->type->vit_dep;
            if(entite->delta.x < 0){
                (entite->pos.x)--;
                entite->delta.x = 0;
            }
            if(hitB(entite,salle)){
                entite->direction = RIGHT;
                (entite->pos.x)++;
                entite->delta.x = 0;
            }
        }
    }
}
//Commentés par MN car ne compile pas (structures pas à jour notamment)
/*
  static int inRange(monstre_t* entite, personnage_t* perso, int radius){
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
/*}

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
        perso->inv = 30;
        entite->direction = 1 - entite->direction;
    }else{
        dep(entite,salle);
    }
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
*/
