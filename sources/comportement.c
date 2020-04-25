#include <stdlib.h>
#include <string.h>
//#include <SDL2/SDL.h>
#include "../headers/structs.h"
#include "../headers/liste.h"
#include "../headers/comportement.h"
#include "../headers/source.h"

/**
 * \file comportement.c
 * \brief Ensemble de fonctions pour le comportement des mobs et du personnage
 * \author Thomas DIDIER L2 Info Le Mans
 * \version 3.5
 * \date 23/04/2020
*/

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
            return;
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

    if(strcmp(e1->type->nom,"fleche") && strcmp(e1->type->nom,"fleche feu"))
        return FALSE;

    if(!strcmp(e2->type->nom,"coeur") || !strcmp(e2->type->nom,"fleche") || !strcmp(e2->type->nom,"fleche feu") || !strcmp(e2->type->nom,"venin"))
        return FALSE;

    if(e2->type->comportement == compRecuperable)
        return FALSE;
    
    if(e2->type->comportement == compPortes)
        if(e2->pv == 2)
            return FALSE;

    int leftE1, leftE2;
    int rightE1, rightE2;
    int topE1, topE2;
    int bottomE1, bottomE2;

    leftE1 = e1->pos.x*TAILLEBLOC + e1->delta.x + (e1->type->tailleSprite.largeur - e1->type->hitbox.largeur)/2;
    topE1 = e1->pos.y*TAILLEBLOC + e1->delta.y + (e1->type->tailleSprite.hauteur - e1->type->hitbox.hauteur)/2;
    rightE1 = leftE1 + e1->type->hitbox.largeur;
    bottomE1 = topE1 + e1->type->hitbox.hauteur;

    leftE2 = e2->pos.x*TAILLEBLOC + e2->delta.x + (e2->type->tailleSprite.largeur - e2->type->hitbox.largeur)/2;
    topE2 = e2->pos.y*TAILLEBLOC + e2->delta.y + (e2->type->tailleSprite.hauteur - e2->type->hitbox.hauteur)/2;
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
static int hitP(monstre_t* m, personnage_t* p){
    int leftE, leftP;
    int rightE, rightP;
    int topE, topP;
    int bottomE, bottomP;

    leftE = m->pos.x*TAILLEBLOC + m->delta.x;
    rightE = leftE + m->type->hitbox.largeur;
    topE = m->pos.y*TAILLEBLOC + m->delta.y + (m->type->tailleSprite.hauteur - m->type->hitbox.hauteur) / (m->type->comportement == compPortes ? 1 : 2);
    bottomE = topE + m->type->hitbox.hauteur;

    leftP = p->pos.x*TAILLEBLOC + p->delta.x + OFFSETHITBOX;
    rightP = leftP + p->hitbox.largeur;
    topP = p->pos.y*TAILLEBLOC + p->delta.y + 1;
    bottomP = topP + p->hitbox.hauteur;

    if(bottomE <= topP)
        return FALSE;

    if(topE >= bottomP)
        return FALSE;

    if(rightE <= leftP)
        return FALSE;

    if(leftE >= rightP)
        return FALSE;

    int centerE = (leftE + rightE) / 2;

    if(centerE > leftP)
        if(centerE > rightP)
            return -1;
        else
            return centerE - leftP > rightP - centerE ? -1 : 1;
    else
        return 1;
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
            }
            break;
        default:
            break;
    }
}

/**
 * \brief Vérifie la case au dessus du personnage
 *
 * @param p pointeur vers le personnage
 * @param s pointeur vers la salle
 * @return 1 (TRUE) si le il y a un bloc, 0 (FALSE) sinon
*/
static int verifCaseUp(personnage_t* p, salle_t* s){
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

/**
 * \brief Vérifie la case en dessous du personnage
 *
 * @param p pointeur vers le personnage
 * @param s pointeur vers la salle
 * @return 1 (TRUE) si le il y a un bloc, 0 (FALSE) sinon
*/
static int verifCaseDown(personnage_t* p, salle_t* s){
    int leftP;
    int rightP;
    int bottomP;

    leftP = p->pos.x*TAILLEBLOC + p->delta.x + OFFSETHITBOX;
    rightP = leftP + p->hitbox.largeur;
    bottomP = p->pos.y*TAILLEBLOC + p->delta.y + 1;
    bottomP += p->hitbox.hauteur + 1;

    leftP /= TAILLEBLOC;
    rightP = rightP/TAILLEBLOC + (rightP%TAILLEBLOC ? 1 : 0) - 1;
    bottomP = bottomP/TAILLEBLOC + (bottomP%TAILLEBLOC ? 1 : 0) - 1;

    if(bottomP >= s->hauteur )
        return -1; //Le personnage est tombé dans un trou

    for(int i = leftP; i <= rightP; i++)
        if(s->mat[bottomP][i])
            return TRUE;

    return FALSE;
}

/**
 * \brief Gère le déplacement vertical du personnage quelque soit son état
 *
 * @param p pointeur vers le personnage
 * @param s pointeur vers la salle
 * @param tryJump booléen qui vaut TRUE(1) si le personnage essaie actuellement de sauter, FALSE(0) sinon
*/
void depVert(personnage_t* p, salle_t* s, int tryJump){
    int idSaut;
    for(idSaut = 0; idSaut < TAILLE_INVENTAIRE && strcmp(p->nomObj[idSaut],"double saut"); idSaut++);
    if(p->jpCd){
        (p->jpCd)--;
    }
    if(p->kb)
        switch(p->etat){
            case IDLE:
            case RUNNING:
                p->etat = JUMPING;
                p->newEtat = TRUE;
                p->nbPxSaut = 0;
                break;
            case JUMPING:
                if(verifCaseUp(p,s)){
                    p->etat = FALLING;
                    p->newEtat = TRUE;
                }
                else{
                    if(p->nbPxSaut >= (NBPXSAUT / 4)){
                        p->etat = FALLING;
                        p->newEtat = TRUE;
                    }else{
                        p->delta.y -= p->vit_saut;
                        if(p->delta.y < 0){
                            (p->pos.y)--;
                            p->delta.y += TAILLEBLOC;
                        }
                        if(!persValidDep(p,s)){
                            //traitement si le pixel juste au dessus est accessible mais pas la position d'arrivé
                            //aka trouver le position de cognement
                            (p->pos.y)++;
                            p->delta.y = 1;
                        }else{
                            p->nbPxSaut += p->vit_saut;
                        }
                    }
                }
                break;
            case FALLING:
                if(verifCaseDown(p,s)){
                    p->etat = IDLE; //défini comme tel pour éviter de sortir de etat_t mais on ne sait pas si il est IDLE ou RUNNING
                    p->newEtat = TRUE;
                    p->jpCd = JPCD/4;
                    p->kb = 0;
                    p->inv = TEMPINV;
                }else{
                    //continuer chute
                    p->delta.y += p->vit_chute;
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
    else
        switch(p->etat){
            case IDLE:
            case RUNNING:
                if(tryJump && !p->jpCd){
                    p->etat = JUMPING;
                    p->newEtat = TRUE;
                    p->nbSaut = 1;
                    p->nbPxSaut = 0;
                }else
                    if(!verifCaseDown(p,s)){
                        p->etat = FALLING;
                        p->nbSaut = 1;
                        p->newEtat = TRUE;
                    }
                break;
            case JUMPING:
                if(verifCaseUp(p,s)){
                    p->etat = FALLING;
                    p->newEtat = TRUE;
                }
                else
                    if(tryJump && p->nbSaut < 1 + p->inventaire[idSaut]){
                        //non testé
                        (p->nbSaut)++;
                        p->nbPxSaut = 0;
                        //trouver comment gerer l'animation de resaut
                    }
                    else
                        if(p->nbPxSaut >= NBPXSAUT){
                            p->etat = FALLING;
                            p->newEtat = TRUE;
                        }else{
                            p->delta.y -= p->vit_saut;
                            if(p->delta.y < 0){
                                (p->pos.y)--;
                                p->delta.y += TAILLEBLOC;
                            }
                            if(!persValidDep(p,s)){
                                //traitement si le pixel juste au dessus est accessible mais pas la position d'arrivé
                                //aka trouver le position de cognement
                                (p->pos.y)++;
                                p->delta.y = 1;
                            }else{
                                p->nbPxSaut += p->vit_saut;
                            }
                        }
                break;
            case FALLING:
                if(verifCaseDown(p,s)){
                    p->etat = IDLE; //défini comme tel pour éviter de sortir de etat_t mais on ne sait pas si il est IDLE ou RUNNING
                    p->nbSaut = 0;
                    p->newEtat = TRUE;
                    p->jpCd = JPCD;
                }else
                    if(tryJump && p->nbSaut < 1 + p->inventaire[idSaut] && !p->jpCd){
                        (p->nbSaut)++;
                        p->nbPxSaut = 0;
                        p->etat = JUMPING;
                        p->newEtat = TRUE;
                    }
                    else{
                        //continuer chute
                        p->delta.y += p->vit_chute;
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

void attaquer(personnage_t* p, salle_t* s, int tryAtk){
    if(p->kb && (p->etat == ATTACKING || tryAtk)){
        if(p->etat == ATTACKING){
            p->etat = IDLE;
            p->newEtat = TRUE;
        }
        p->nbFrameAtk = 0;
    }else{
        if(p->etat == ATTACKING){
            if(p->nbFrameAtk == p->vit_att){
                monstre_t* f = malloc(sizeof(monstre_t));

                int i;
                for(i = 0; i < TAILLE_INVENTAIRE && strcmp(p->nomObj[i],"huile"); i++);
                f->type = i < TAILLE_INVENTAIRE && p->inventaire[i] ? &typesMonstre[-FLECHEFEU - 1] : &typesMonstre[-FLECHE - 1];
                
                f->pv = f->type->pv_base;

                int leftP = p->pos.x*TAILLEBLOC + p->delta.x + OFFSETHITBOX;
                int rightP = leftP + p->hitbox.largeur;

                if(p->direction){
                    f->delta = (position_t){rightP%8,1};
                    f->pos = (position_t){rightP/8,p->pos.y+2};
                    f->direction = RIGHT;
                }else{
                    f->delta = (position_t){leftP%8,1};
                    f->pos = (position_t){leftP/8 - 1,p->pos.y+2};
                    f->direction = LEFT;
                }

                f->delta.x += f->type->vit_dep * f->direction ? 1 : -1;

                if(f->direction){
                    if(f->delta.x >= TAILLEBLOC){
                        (f->pos.x)++;
                        f->delta.x -= TAILLEBLOC;
                    }
                }else{
                    if(f->delta.x < 0){
                        (f->pos.x)--;
                        f->delta.x += TAILLEBLOC;
                    }
                }

                f->etat = RUNNING;

                f->spriteActuel.h = f->type->tailleSprite.hauteur;
                f->spriteActuel.w = f->type->tailleSprite.largeur;
                f->spriteActuel.x = 0;
                //f->spriteActuel.y = 0;
                f->spriteActuel.y = f->etat * f->spriteActuel.h;

                enTete(s->listeEntite);
                ajoutDroit(s->listeEntite, f);

                p->etat = IDLE;
                p->newEtat = TRUE;
                p->nbFrameAtk = 0;
            }else{
                (p->nbFrameAtk)++;
            }
        }else{
            if(tryAtk && p->etat <= RUNNING){
                p->etat = ATTACKING;
                p->newEtat = TRUE;
                p->nbFrameAtk = 0;
            }
        }
    }
}

/**
 * \brief Estime si le joueur touche une porte et rend l'endroit où elle l'emmène
 *
 * @param p pointeur vers le personnage
 * @param lPortes pointeur vers la liste de portes à vérifier
 * @return une chaine de caractères correspondant au fichier texte à charger (nouvelle salle)
*/
char* prendPorte(personnage_t* p, liste_t* lPortes){
    if(strcmp(lPortes->type,"porte"))
        return NULL;

    porte_t porte;
    char* salle = NULL;
    int i = 1;

    enTete(lPortes);
    while(!horsListe(lPortes)){
        valeurElm(lPortes,&porte);
        int leftPo = porte.pos.x * TAILLEBLOC;
        int rightPo = leftPo + TAILLEBLOC;
        int topPo = porte.pos.y * TAILLEBLOC;
        int bottomPo = topPo + TAILLEBLOC;

        int leftPe = p->pos.x*TAILLEBLOC + p->delta.x + OFFSETHITBOX;
        int rightPe = leftPe + p->hitbox.largeur;
        int topPe = p->pos.y*TAILLEBLOC + p->delta.y + 1;
        int bottomPe = topPe + p->hitbox.hauteur;

        if(rightPe >= rightPo && leftPe <= leftPo && topPe <= topPo && bottomPe >= bottomPo){
            p->pos = porte.pos_arrivee;
            p->delta.x = 2;
            salle = malloc(sizeof(char)*(strlen(porte.salleSuivante)+1));
            strcpy(salle,porte.salleSuivante);
            return salle;
        }
        suivant(lPortes);
    }
    return NULL;
}

/**
 * \brief Vérifie si une entité est dans un bloc que la salle
 *
 * @param e pointeur vers l'entité
 * @param s pointeur vers la salle
 *
 * @return 1 (TRUE) si il y a contact, 0 (FALSE) sinon
*/
static int hitB(monstre_t* m, salle_t* s){
    int leftM;
    int rightM;
    int topM;
    int bottomM;

    leftM = m->pos.x*TAILLEBLOC + m->delta.x;
    rightM = leftM + m->type->hitbox.largeur - (m->type->tailleSprite.largeur - m->type->hitbox.largeur)/2;
    topM = m->pos.y*TAILLEBLOC + m->delta.y + 1;
    bottomM = topM + m->type->hitbox.hauteur;

    if(leftM < 0 || topM < 0)
        return TRUE;

    leftM /= 8;
    rightM = rightM/8 + (rightM%8 ? 1 : 0) - 1;
    topM /= 8;
    bottomM = bottomM/8 + (bottomM%8 ? 1 : 0) - 1;

    if(bottomM >= s->hauteur || rightM >= s->largeur)
        return TRUE;

    for(int i = leftM; i <= rightM; i++)
        for(int j = topM; j <= bottomM; j++)
            if(s->mat[j][i]){
                return TRUE;
            }

    return FALSE;
}

/**
 * \brief Vérifie si le monstre doit chuter
 *
 * @param m pointeur vers le monstre
 * @param s pointeur vers la salle
 * @return 1 (TRUE) si le monstre doit chuter, 0 (FALSE) sinon
*/
static int verifChuteMonstre(monstre_t* m, salle_t* s){
    int leftM;
    int rightM;
    int bottomM;

    leftM = m->pos.x*TAILLEBLOC + m->delta.x;
    rightM = leftM + m->type->hitbox.largeur - (m->type->tailleSprite.largeur - m->type->hitbox.largeur)/2;
    bottomM = m->pos.y*TAILLEBLOC + m->delta.y;
    bottomM += m->type->hitbox.hauteur + 1;

    leftM /= TAILLEBLOC;
    rightM = rightM/TAILLEBLOC + (rightM%TAILLEBLOC ? 1 : 0) - 1;
    bottomM = bottomM/TAILLEBLOC + (bottomM%TAILLEBLOC == 7 ? 1 : 0);

    if(bottomM >= s->hauteur)
        return -1; //Le monstre est tombé dans le vide ?

    for(int i = leftM; i <= rightM; i++){
        if(!s->mat[bottomM][i])
            return TRUE;
    }

    return FALSE;
}

/**
 * \brief Gère le déplacement d'une entite (monstre)
 *
 * @param entite le pointeur vers la structure monstre à déplacer
 * @param salle le pointeur vers la structure salle où se trouve l'entite
*/
static int dep(monstre_t* entite, salle_t* salle, boolean_t chute){
    if(entite->pv){
        if(entite->direction){
            entite->delta.x += entite->type->vit_dep;
            if(entite->delta.x >= TAILLEBLOC){
                (entite->pos.x)++;
                entite->delta.x -= TAILLEBLOC;
            }
            if(hitB(entite,salle) || (verifChuteMonstre(entite,salle) && chute))
                return TRUE;
            return FALSE;
        }else{
            entite->delta.x -= entite->type->vit_dep;
            if(entite->delta.x < 0){
                (entite->pos.x)--;
                entite->delta.x += TAILLEBLOC;
            }
            if(hitB(entite,salle) || (verifChuteMonstre(entite,salle) && chute))
                return TRUE;
            return FALSE;
        }
    }
    return FALSE;
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
}*/

void compRecuperable(monstre_t* entite, personnage_t* perso, salle_t* salle){
    entite->etat = IDLE;
    if(hitP(entite,perso))
        recupElem(entite,perso);
}

void compCoeur(monstre_t* e, personnage_t* p, salle_t* s){
    if(hitP(e,p)){
        p->pv += e->type->degat;
        e->pv = 0;
    }else{
        if(e->type->vit_dep == 1){
            e->delta.y += e->type->vit_dep;
            if(e->delta.y >= TAILLEBLOC){
                (e->pos.y)++;
                e->delta.y -= TAILLEBLOC;
            }
            if(hitB(e,s)){
                (e->pos.y)--;
                e->delta.y = 7;
            }
            e->type->vit_dep = 3;
        }else
            e->type->vit_dep--;
    }
}

void compFleches(monstre_t* entite, personnage_t* perso, salle_t* salle){
    monstre_t tmp;
    enTete(salle->listeEntite);
    while(!horsListe(salle->listeEntite)){
        valeurElm(salle->listeEntite,&tmp);
        if(hitE(entite,&tmp)){
            if(tmp.type->comportement != compPortes){
                tmp.pv -= entite->type->degat;
                modifElm(salle->listeEntite,&tmp);
            }
            entite->pv = 0;
            entite->etat = IDLE;
            //entite->newEtat = TRUE;
        }
        suivant(salle->listeEntite);
    }

    if(dep(entite,salle,FALSE)){
        entite->pv = 0;
        entite->etat = IDLE;
        //entite->newEtat = TRUE;
    }
}

void compPortes(monstre_t* e, personnage_t* p, salle_t* s){
    if(!e->direction)
        e->direction = RIGHT;
    
    if(e->pv == 1 && e->etat != IDLE)
        e->etat = IDLE;

    int aie = hitP(e,p);
    if(aie && e->pv == 1){
        int i, l;
        int validate = 0;
        char tmp1[10], tmp2[10];

        for(l = 0; l < strlen(e->type->nom) && e->type->nom[l] != ' '; l++);
        strcpy(tmp1, e->type->nom + l + 1);

        for(i = 0; i < TAILLE_INVENTAIRE && !validate; i++){
            strcpy(tmp2, "");
            for(l = 0; l < strlen(p->nomObj[i]) && p->nomObj[i][l] != ' '; l++);
            if(l < strlen(p->nomObj[i])){
                strcpy(tmp2, p->nomObj[i] + l + 1);
                if(!strcmp(tmp1,tmp2))
                    validate = 1;
            }
        }
        if(validate && p->inventaire[i-1]){
            //changement d'état
            e->pv = 2;
            e->etat = RUNNING;
        }else{
            while(aie){
                p->delta.x += aie;
                aie = hitP(e,p);
            }
        }

    }
}

void compMurGlace(monstre_t* entite, personnage_t* perso, salle_t* salle){
    if(entite->pv)
        entite->pv = 2;
}

void compRoiVifplume(monstre_t* entite, personnage_t* perso, salle_t* salle){
    //fonction en beta
    /*if(hitP(entite,perso)){
        perso->pv -= entite->type->degat;
    }
    if(!hitB(entite,salle)){
        dep(entite);
    }*/
}

void compSerpent(monstre_t* entite, personnage_t* perso, salle_t* salle){
    int dir = hitP(entite,perso);
    if(dir && !perso->inv && !perso->kb){
        perso->pv -= entite->type->degat;
        perso->kb = 1;
        perso->etat = IDLE;
        entite->direction = dir > 0 ? 0 : 1;
        perso->direction = entite->direction;
    }else{
        //(entite->ut)--;
        printf("%p_%p:",entite, &(entite->ut));
        printf("%d\n",entite->ut);
        if(entite->ut <= 0){
            if(dep(entite,salle,TRUE)){
                if(entite->direction){
                    entite->direction = LEFT;
                    entite->delta.x -= entite->type->vit_dep;
                    if(entite->delta.x < 0){
                        (entite->pos.x)--;
                        entite->delta.x += TAILLEBLOC;
                    }
                }else{
                    entite->direction = RIGHT;
                    entite->delta.x += entite->type->vit_dep;

                    if(entite->delta.x >= TAILLEBLOC){
                        (entite->pos.x)++;
                        entite->delta.x -= TAILLEBLOC;
                    }
                }
            }
            //entite->ut = 2;
        }
    }
}

void compSerpentRose(monstre_t* entite, personnage_t* perso, salle_t* salle){
    /*if(hitP(entite,perso)){
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
    }*/
}

void compSingeGrotte(monstre_t* entite, personnage_t* perso, salle_t* salle){
    entite->etat = IDLE;
    /*if(inRange(entite,perso,4)){
        //attaquer
    }*/
}

void compVersGeant(monstre_t* entite, personnage_t* perso, salle_t* salle){

}

void compVifplume(monstre_t* entite, personnage_t* perso, salle_t* salle){
    entite->etat = IDLE;
}

static void creerCoeur(monstre_t* m, salle_t* s){
    monstre_t* c = malloc(sizeof(monstre_t));
    c->type = &typesMonstre[-COEUR - 1];
    c->pv = c->type->pv_base;

    int leftP = m->pos.x*TAILLEBLOC + m->delta.x + OFFSETHITBOX;
    int rightP = leftP + m->type->hitbox.largeur;

    c->delta = (position_t){((leftP + rightP)/2)%8,1};
    c->pos = (position_t){(leftP + rightP)/16, m->pos.y};
    c->direction = 1;

    c->etat = IDLE;

    c->spriteActuel.h = c->type->tailleSprite.hauteur;
    c->spriteActuel.w = c->type->tailleSprite.largeur;
    c->spriteActuel.x = 0;
    c->spriteActuel.y = c->etat * c->spriteActuel.h;

    enTete(s->listeEntite);
    ajoutDroit(s->listeEntite, c);
}

void evolution(personnage_t* p, salle_t* s){
    int r;
    if(p->inv)
        (p->inv)--;
    if(p->kb){
        if(p->direction){
            depGauche(p,s);
        }else{
            depDroite(p,s);
        }
    }
    s->listeEntite->indTmp = 0;
    monstre_t e;
    enTete(s->listeEntite);
    while(!horsListe(s->listeEntite)){
        valeurElm(s->listeEntite,&e);
        if(e.pv > 0){
            if(e.type->comportement)
                e.type->comportement(&e,p,s);

            enTete(s->listeEntite);
            for(int i = 0; i < s->listeEntite->indTmp; i++)
                suivant(s->listeEntite);

            modifElm(s->listeEntite,&e);
            (s->listeEntite->indTmp)++;
        }else{
            oterElm(s->listeEntite,supMonstre); //je reviens au précédent après avoir oté

            //creation des coeurs
            if(e.type->comportement != compFleches && strcmp(e.type->nom,"venin") && e.type->comportement != compRecuperable && e.type->comportement != compCoeur){
                r = rand() % 100;
                if(r < COEURDROPRATE){
                    //creer un coeur
                    creerCoeur(&e,s);
                }
            }
        }
        suivant(s->listeEntite);
    }
    if(p->pv < 0)
        p->pv = 0;
    else
        if(p->pv > p->pv_max)
            p->pv = p->pv_max;
}
