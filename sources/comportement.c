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
 * \version 3.1
 * \date 06/04/2020
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

    if(strcmp(e1->type->nom,"fleche") && strcmp(e1->type->nom,"fleche_feu"))
        return FALSE;

    if(!strcmp(e2->type->nom,"fleche") || !strcmp(e2->type->nom,"fleche_feu") || !strcmp(e2->type->nom,"crachat"))
        return FALSE;

    int leftE1, leftE2;
    int rightE1, rightE2;
    int topE1, topE2;
    int bottomE1, bottomE2;

    leftE1 = e1->pos.x*TAILLEBLOC + e1->delta.x + (e1->type->tailleSprite.largeur - e1->type->hitbox.largeur)/2;
    topE1 = e1->pos.y*TAILLEBLOC + e1->delta.y + (e1->type->tailleSprite.hauteur - e1->type->hitbox.hauteur)/2;;
    rightE1 = leftE1 + e1->type->hitbox.largeur;
    bottomE1 = topE1 + e1->type->hitbox.hauteur;

    leftE2 = e2->pos.x*TAILLEBLOC + e2->delta.x + (e2->type->tailleSprite.largeur - e2->type->hitbox.largeur)/2;
    topE2 = e2->pos.y*TAILLEBLOC + e2->delta.y + (e2->type->tailleSprite.hauteur - e2->type->hitbox.hauteur)/2;;
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

    leftE = m->pos.x*TAILLEBLOC + m->delta.x + (m->type->tailleSprite.largeur - m->type->hitbox.largeur)/2;
    rightE = leftE + m->type->hitbox.largeur;
    topE = m->pos.y*TAILLEBLOC + m->delta.y + (m->type->tailleSprite.hauteur - m->type->hitbox.hauteur)/2;
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

    return TRUE;
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

/**
 * \brief Gère le déplacement vertical du personnage quelque soit son état
 *
 * @param p pointeur vers le personnage
 * @param s pointeur vers la salle
 * @param tryJump booléen qui vaut TRUE(1) si le personnage essaie actuellement de sauter, FALSE(0) sinon
*/
void depVert(personnage_t* p, salle_t* s, int tryJump){
    if(p->jpCd){
        (p->jpCd)--;
    }
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
                if(tryJump && p->nbSaut < 1 + p->inventaire[6]){
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
                if(tryJump && p->nbSaut < 1 + p->inventaire[6] && !p->jpCd){
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
    if(p->etat == ATTACKING){
        if(p->nbFrameAtk == p->vit_att){
            monstre_t* f = malloc(sizeof(monstre_t));
            f->type = &typesMonstre[-FLECHE - 1];
            f->pv = f->type->pv_base;

            int leftP = p->pos.x*TAILLEBLOC + p->delta.x + OFFSETHITBOX;
            int rightP = leftP + p->hitbox.largeur;

            if(p->direction){
                f->delta = (position_t){rightP%8,0};
                f->pos = (position_t){rightP/8,p->pos.y+1};
                f->direction = RIGHT;
            }else{
                f->delta = (position_t){leftP%8,0};
                f->pos = (position_t){leftP/8,p->pos.y+1};
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
            f->spriteActuel.y = 0;
            //f->spriteActuel.y = f->etat * f->spriteActuel.h;

            enTete(s->listeEntite);
            ajoutGauche(s->listeEntite, f);

            p->etat = IDLE;
            p->newEtat = TRUE;
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
    bottomM = m->pos.y*TAILLEBLOC + m->delta.y + 1;
    bottomM += m->type->hitbox.hauteur;

    leftM /= 8;
    rightM = rightM/8 + (rightM%8 ? 1 : 0) - 1;
    bottomM = bottomM/8 + (bottomM%8 ? 1 : 0);

    if(bottomM >= s->hauteur )
        return -1; //Le monstre est tombé dans le vide ?

    for(int i = leftM; i <= rightM; i++){
        if(s->mat[bottomM][i])
            return FALSE;
    }

    return TRUE;
}

/**
 * \brief Gère le déplacement d'une entite (monstre)
 *
 * @param entite le pointeur vers la structure monstre à déplacer
 * @param salle le pointeur vers la structure salle où se trouve l'entite
*/
static int dep(monstre_t* entite, salle_t* salle){
    if(entite->pv){
        if(entite->direction){
            entite->delta.x += entite->type->vit_dep;
            if(entite->delta.x >= TAILLEBLOC){
                (entite->pos.x)++;
                entite->delta.x -= TAILLEBLOC;
            }
            if(hitB(entite,salle) || verifChuteMonstre(entite,salle))
                return TRUE;
            return FALSE;
        }else{
            entite->delta.x -= entite->type->vit_dep;
            if(entite->delta.x < 0){
                (entite->pos.x)--;
                entite->delta.x += TAILLEBLOC;
            }
            if(hitB(entite,salle) || verifChuteMonstre(entite,salle))
                return TRUE;
            return FALSE;
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
}*/

void compFleches(monstre_t* entite, personnage_t* perso, salle_t* salle){
    monstre_t tmp;
    enTete(salle->listeEntite);
    while(!horsListe(salle->listeEntite)){
        valeurElm(salle->listeEntite,&tmp);
        if(hitE(entite,&tmp)){
            tmp.pv -= entite->type->degat;
            modifElm(salle->listeEntite,&tmp);
            entite->pv = 0;
            entite->etat = IDLE;
            entite->newEtat = TRUE;
        }
        suivant(salle->listeEntite);
    }

    if(dep(entite,salle)){
        entite->pv = 0;
        entite->etat = IDLE;
        entite->newEtat = TRUE;
    }
}

/*void compMurGlace(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
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
}*/

void compSerpent(monstre_t* entite, personnage_t* perso, salle_t* salle){
    if(hitP(entite,perso) && !perso->inv){
        perso->pv -= entite->type->degat;
        perso->inv = 60;
        entite->direction = 1 - entite->direction;
    }else{
        entite->type->vit_dep = 1 - entite->type->vit_dep; //divise par deux la vitesse de dep du serpent

        if(perso->inv)
            (perso->inv)--;
        
        if(dep(entite,salle)){
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
    }
}

/*void compSerpentRose(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
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

void evolution(personnage_t* p, salle_t* s){
    monstre_t e;
    enTete(s->listeEntite);
    while(!horsListe(s->listeEntite)){
        valeurElm(s->listeEntite,&e);
        if(e.pv){
            e.type->comportement(&e,p,s);
            modifElm(s->listeEntite,&e);
        }else{
            oterElm(s->listeEntite,supMonstre); //je reviens au précédent après avoir oté

            //creation des coeurs
            /*if(strcmp(e.type->nom,"fleche") && strcmp(e.type->nom,"fleche_feu") && strcmp(e.type->nom,"crachat") && e.type->comportement != compRecuperable){
                srand(time(NULL));
                int r = rand() % 100;
                if(r < COEURDROPRATE){
                    //creer un coeur
                }
            }*/
        }
        suivant(s->listeEntite);
    }
}