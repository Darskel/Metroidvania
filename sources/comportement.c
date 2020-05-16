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
    rect1.w = e1->type->hitbox.w;
    rect1.h = e1->type->hitbox.h;

    rect2.x = TAILLE_BLOCK*e2->pos.x + e2->delta.x;
    rect2.y = TAILLE_BLOCK*e2->pos.y + e2->delta.y;
    rect2.w = e2->type->hitbox.w;
    rect2.h = e2->type->hitbox.h;

    return SDL_IntersectRect(&rect1,&rect2);
}*/

static int puissance(int a, int b){
    int som = 1;

    for(int i = 0; i < b; i++)
        som *= a;

    return som;
}

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
            perso->newItem=TRUE;
            entite->pv = 0;
            if(!(perso->sounds & puissance(2,SOUND_ITEM)))
                perso->sounds += puissance(2,SOUND_ITEM);
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
    SDL_Rect e1H;
    SDL_Rect e2H;

    if(strcmp(e1->type->nom,"fleche") && strcmp(e1->type->nom,"fleche feu"))
        return FALSE;

    if(!strcmp(e2->type->nom,"coeur") || !strcmp(e2->type->nom,"fleche") || !strcmp(e2->type->nom,"fleche feu") || !strcmp(e2->type->nom,"venin"))
        return FALSE;

    if(e2->type->comportement == compRecuperable)
        return FALSE;

    if(e2->type->comportement == compPortes)
        if(e2->pv == 2)
            return FALSE;

    for(int i=0; i<e1->type->nbHitbox; i++){
      e1H.w=e1->type->hitbox[e1->etat + i*NBETATS].w;
      e1H.h=e1->type->hitbox[e1->etat + i*NBETATS].h;
      e1H.y=e1->pos.y*TAILLEBLOC + e1->delta.y + e1->type->hitbox[e1->etat + i*NBETATS].y;
      e1H.x=e1->pos.x*TAILLEBLOC + e1->delta.x;
      if(e1->direction){
        e1H.x+=e1->type->tailleSprite.largeur - e1->type->hitbox[e1->etat + i*NBETATS].x - e1H.w ;
      }
      else{
        e1H.x+=e1->type->hitbox[e1->etat + i*NBETATS].x;
      }

      for(int j=0; j<e2->type->nbHitbox; j++){
        e2H.w=e2->type->hitbox[e2->etat + j*NBETATS].w;
        e2H.h=e2->type->hitbox[e2->etat + j*NBETATS].h;
        e2H.y=e2->pos.y*TAILLEBLOC + e2->delta.y + e2->type->hitbox[e2->etat + j*NBETATS].y;
        e2H.x=e2->pos.x*TAILLEBLOC + e2->delta.x;
        if(e2->direction){
          e2H.x+=e2->type->tailleSprite.largeur - e2->type->hitbox[e2->etat + j*NBETATS].x - e2H.w ;
        }
        else{
          e2H.x+=e2->type->hitbox[e2->etat + j*NBETATS].x;
        }
        if(SDL_HasIntersection(&e1H, &e2H))
          return TRUE;
      }
    }

    return FALSE;
}

/**
 * \brief Vérifie si une entité touche une autre entité
 *
 * @param e1 pointeur vers la première entité
 * @param e2 pointeur vers la seconde entité
 *
 * @return 1 (TRUE) si il y a contact, 0 (FALSE) sinon
*/
int hitP(monstre_t* m, personnage_t* p){
    SDL_Rect mH;
    SDL_Rect pH;
    int intersection = 0;

    pH.w=p->hitbox[p->etat + p->hitboxActuelle*NBETATS].w;
    pH.h=p->hitbox[p->etat + p->hitboxActuelle*NBETATS].h;
    pH.y=p->pos.y*TAILLEBLOC + p->delta.y + p->hitbox[p->etat + p->hitboxActuelle*NBETATS].y;
    pH.x=p->pos.x*TAILLEBLOC + p->delta.x;
    if(p->direction){
      pH.x+=p->spriteActuel.w - p->hitbox[p->etat + p->hitboxActuelle*NBETATS].x - pH.w ;
    }
    else{
      pH.x+=p->hitbox[p->etat + p->hitboxActuelle*NBETATS].x;
    }

    for(int i=0; i<m->type->nbHitbox && !intersection; i++){
      mH.w=m->type->hitbox[m->etat+ i*NBETATS].w;
      mH.h=m->type->hitbox[m->etat+ i*NBETATS].h;
      mH.y=m->pos.y*TAILLEBLOC + m->delta.y + m->type->hitbox[m->etat+ i*NBETATS].y;
      mH.x=m->pos.x*TAILLEBLOC + m->delta.x;
      if(m->direction){
        mH.x+=m->type->tailleSprite.largeur - m->type->hitbox[m->etat+ i*NBETATS].x - mH.w ;
      }
      else{
        mH.x+=m->type->hitbox[m->etat+ i*NBETATS].x;
      }
      if(SDL_HasIntersection(&mH, &pH)){
        intersection = 1;
        if(pH.x + pH.w/2 < mH.x + mH.w/2)
          intersection = -1;
      }
    }

    return intersection;

}


/**
 * \brief Vérifie si le deplacement du personnage est valide
 *
 * @param p pointeur vers le personnage
 * @param s pointeur vers la salle
 *
 * @return 1 (TRUE) si le déplacement est valide, 0 (FALSE) sinon
*/
int persValid(personnage_t* p, salle_t* s){
    SDL_Rect pH;
    SDL_Rect bloc;
    bloc.w=TAILLEBLOC;
    bloc.h=TAILLEBLOC;

    pH.w=p->hitbox[p->etat + p->hitboxActuelle*NBETATS].w;
    pH.h=p->hitbox[p->etat + p->hitboxActuelle*NBETATS].h;
    pH.y=p->pos.y*TAILLEBLOC + p->delta.y + p->hitbox[p->etat + p->hitboxActuelle*NBETATS].y;
    pH.x=p->pos.x*TAILLEBLOC + p->delta.x;
    if(p->direction){
      pH.x+=p->spriteActuel.w - p->hitbox[p->etat + p->hitboxActuelle*NBETATS].x - pH.w ;
    }
    else{
      pH.x+=p->hitbox[p->etat + p->hitboxActuelle*NBETATS].x;
    }

    if(pH.y + pH.h >= s->hauteur*TAILLEBLOC|| pH.x + pH.w >= s->largeur*TAILLEBLOC || pH.x < 0 || pH.y < 0)
        return FALSE;

    for(int i = 0; i < s->largeur; i++){
        for(int j = 0; j < s->hauteur; j++){
          if(s->mat[j][i]){
            bloc.x=i*bloc.w;
            bloc.y=j*bloc.h;
            if(SDL_HasIntersection(&pH, &bloc)){
              return FALSE;
            }
          }
        }
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
    int oldposx=p->pos.x;
    int olddeltax=p->delta.x;
    switch(p->etat){
        case IDLE:
        case RUNNING:
        case JUMPING:
        case FALLING:
            p->delta.x += p->vit_dep;
            if(p->delta.x >= TAILLEBLOC){
                (p->pos.x)++;
                p->delta.x -= TAILLEBLOC;
            }
            if(persValid(p,s)){
              if(p->etat==IDLE){
                p->etat = RUNNING;
                p->newEtat = TRUE;
              }
            }
            else{
                p->delta.x = olddeltax;
                p->pos.x = oldposx;
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
  int oldposx=p->pos.x;
  int olddeltax=p->delta.x;
    switch(p->etat){
        case IDLE:
        case RUNNING:
        case JUMPING:
        case FALLING:
            p->delta.x -= p->vit_dep;
            if(p->delta.x < 0){
                (p->pos.x)--;
                p->delta.x += TAILLEBLOC;
            }
            if(persValid(p,s)){
              if(p->etat==IDLE){
                p->etat = RUNNING;
                p->newEtat = TRUE;
              }
            }
            else{
                p->pos.x = oldposx;
                p->delta.x = olddeltax;
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
/*static int verifCaseUp(personnage_t* p, salle_t* s){
    int leftP;
    int rightP;
    int topP;

    leftP = p->pos.x*TAILLEBLOC + p->delta.x + p->hitbox.x;
    rightP = leftP + p->hitbox.w;
    topP = p->pos.y*TAILLEBLOC + p->hitbox.y + p->delta.y;

    if(topP < 0)
        return TRUE;

    leftP /= TAILLEBLOC;
    rightP = rightP/TAILLEBLOC;
    topP = topP/TAILLEBLOC - (topP%TAILLEBLOC ? 1 : 0);

    for(int i = leftP; i <= rightP; i++)
        if(s->mat[topP][i])
            return TRUE;

    return FALSE;
}*/

/**
 * \brief Vérifie la case en dessous du personnage
 *
 * @param p pointeur vers le personnage
 * @param s pointeur vers la salle
 * @return 1 (TRUE) si le il y a un bloc, 0 (FALSE) sinon
*/
/*static int verifCaseDown(personnage_t* p, salle_t* s){
    int leftP;
    int rightP;
    int bottomP;

    leftP = p->pos.x*TAILLEBLOC + p->delta.x + p->hitbox.x;
    rightP = leftP + p->hitbox.w;
    bottomP = p->pos.y*TAILLEBLOC + p->delta.y + p->hitbox.y + p->hitbox.h;

    leftP /= TAILLEBLOC;
    rightP = rightP/TAILLEBLOC;
    bottomP = bottomP/TAILLEBLOC + (bottomP%TAILLEBLOC ? 1 : 0);

    if(bottomP >= s->hauteur )
        return -1; //Le personnage est tombé dans un trou

    for(int i = leftP; i <= rightP; i++)
        if(s->mat[bottomP][i])
            return TRUE;

    return FALSE;
}*/

/**
 * \brief Gère le déplacement vertical du personnage quelque soit son état
 *
 * @param p pointeur vers le personnage
 * @param s pointeur vers la salle
 * @param tryJump booléen qui vaut TRUE(1) si le personnage essaie actuellement de sauter, FALSE(0) sinon
*/
void depVert(personnage_t* p, salle_t* s, int tryJump){
    int idSaut;
    int oldposy = p->pos.y;
    int olddeltay = p->delta.y;
    etat_t oldetat = p->etat;
    int tailleSaut = NBPXSAUT;
    if(p->forme=='r')
      tailleSaut = 3*TAILLEBLOC;
    for(idSaut = 0; idSaut < TAILLE_INVENTAIRE && strcmp(p->nomObj[idSaut],"double saut"); idSaut++);
    if(p->jpCd){
        (p->jpCd)--;
    }
    if(p->kb){
        switch(p->etat){
            case IDLE:
            case RUNNING:
                p->etat = JUMPING;
                p->newEtat = TRUE;
                p->nbPxSaut = 0;
                p->nbSaut = 1;
                break;
            case JUMPING:
                if(p->nbPxSaut >= (tailleSaut / 3)){
                  p->etat = FALLING;
                  p->newEtat = TRUE;
                }else{
                  if(p->direction==LEFT)
                    depDroite(p,s);
                  if(p->direction==RIGHT)
                    depGauche(p,s);
                  p->delta.y -= p->vit_saut*2;
                  if(p->delta.y < 0){
                    (p->pos.y)--;
                    p->delta.y += TAILLEBLOC;
                  }
                  if(!persValid(p,s)){
                    p->pos.y= oldposy;
                    p->delta.y= olddeltay;
                    if(p->direction==LEFT)
                      depDroite(p,s);
                    if(p->direction==RIGHT)
                      depGauche(p,s);
                    p->etat = FALLING;
                    p->newEtat = TRUE;
                  }else{
                      oldposy=p->pos.y;
                      olddeltay=p->delta.y;
                      p->nbPxSaut += p->vit_saut*2;
                    }
                  }
                break;
            case FALLING:
                //continuer chute
                if(p->direction==LEFT)
                  depDroite(p,s);
                if(p->direction==RIGHT)
                  depGauche(p,s);
                p->delta.y += p->vit_chute*2;
                if(p->delta.y >= TAILLEBLOC){
                  (p->pos.y)++;
                  p->delta.y -= TAILLEBLOC;
                }
                if(!persValid(p,s)){
                  p->pos.y=oldposy;
                  p->delta.y = olddeltay;
                  if(p->direction==LEFT)
                    depDroite(p,s);
                  if(p->direction==RIGHT)
                    depGauche(p,s);
                  p->etat = IDLE; //défini comme tel pour éviter de sortir de etat_t mais on ne sait pas si il est IDLE ou RUNNING
                  p->newEtat = TRUE;
                  p->nbPxSaut = 0;
                  p->kb = 0;
                  p->inv = TEMPINV;
                  p->nbSaut = 0;
                }
                else{
                  oldposy=p->pos.y;
                  olddeltay=p->delta.y;
                }
                break;
            }
          }
    else{
        switch(p->etat){
            case IDLE:
            case RUNNING:
            case ATTACKING:
                if(tryJump && !p->jpCd){
                    p->etat = JUMPING;
                    p->newEtat = TRUE;
                    p->nbSaut = 1;
                    p->nbPxSaut = 0;
                }else{
                    p->etat = JUMPING;
                    p->delta.y += p->vit_chute;
                    if(p->delta.y >= TAILLEBLOC){
                        (p->pos.y)++;
                        p->delta.y -= TAILLEBLOC;
                    }
                    if(persValid(p,s)){
                      p->etat = FALLING;
                      p->nbSaut = 1;
                      p->nbPxSaut = 0;
                      p->newEtat = TRUE;
                      oldposy=p->pos.y;
                      olddeltay=p->delta.y;
                    }
                    else{
                      p->etat = oldetat;
                      p->pos.y=oldposy;
                      p->delta.y=olddeltay;
                    }
                  }
                break;
            case JUMPING:
                if(tryJump && p->nbSaut < 1 + p->inventaire[idSaut]){
                    (p->nbSaut)++;
                    p->nbPxSaut = 0;
                  }
                else{
                    if(p->nbPxSaut >= tailleSaut){
                            p->etat = FALLING;
                            p->newEtat = TRUE;
                            p->nbPxSaut = 0;
                        }else{
                            p->delta.y -= p->vit_saut;
                            if(p->delta.y < 0){
                                (p->pos.y)--;
                                p->delta.y += TAILLEBLOC;
                            }
                            if(!persValid(p,s)){
                                p->pos.y = oldposy;
                                p->delta.y = olddeltay;
                                p->etat = FALLING;
                                p->newEtat = TRUE;
                                p->nbPxSaut = 0;
                            }else{
                                p->nbPxSaut += p->vit_saut;
                                oldposy=p->pos.y;
                                olddeltay=p->delta.y;
                            }
                        }
                    }
                break;
            case FALLING:
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
                      if(!persValid(p,s)){
                          //traitement si le pixel juste en dessous est accessible mais pas la position d'arrivé
                          //aka trouver le position de cognement
                          p->pos.y= oldposy;
                          p->delta.y = olddeltay;
                          p->etat = IDLE; //défini comme tel pour éviter de sortir de etat_t mais on ne sait pas si il est IDLE ou RUNNING
                          p->nbSaut = 0;
                          p->nbPxSaut = 0;
                          p->newEtat = TRUE;
                          p->jpCd = JPCD;
                        }
                      else{
                        oldposy=p->pos.y;
                        olddeltay=p->delta.y;
                      }
                    }
                break;
        }
      }
}

void attaquer(personnage_t* p, salle_t* s, int tryAtk){
  SDL_Rect pH;
    if(p->forme == 'r')
        return;
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


                pH.w=p->hitbox[p->etat + p->hitboxActuelle*NBETATS].w;
                pH.h=p->hitbox[p->etat + p->hitboxActuelle*NBETATS].h;
                pH.y=p->pos.y*TAILLEBLOC + p->delta.y + p->hitbox[p->etat + p->hitboxActuelle*NBETATS].y;
                pH.x=p->pos.x*TAILLEBLOC + p->delta.x;
                if(p->direction){
                  pH.x+=p->spriteActuel.w - p->hitbox[p->etat + p->hitboxActuelle*NBETATS].x - pH.w ;
                  f->delta.x=0;
                  f->delta.y=(pH.y + pH.h/2)%TAILLEBLOC;
                  f->pos.x=(pH.x + pH.w)/TAILLEBLOC;
                  f->pos.y=(pH.y + pH.h/2)/TAILLEBLOC -1;
                  f->delta.y+=2;
                  if(f->delta.y>=TAILLEBLOC) {
                    f->delta.y-=TAILLEBLOC;
                    f->pos.y++;
                  }
                  f->direction = RIGHT;
                }
                else{
                  pH.x+=p->hitbox[p->etat + p->hitboxActuelle*NBETATS].x;
                  f->delta.x=0;
                  f->delta.y=(pH.y + pH.h/2)%TAILLEBLOC;
                  f->pos.x=(pH.x)/TAILLEBLOC;
                  f->pos.y=(pH.y + pH.h/2)/TAILLEBLOC -1;
                  f->delta.y+=2;
                  if(f->delta.y>=TAILLEBLOC) {
                    f->delta.y-=TAILLEBLOC;
                    f->pos.y++;
                  }
                  f->direction = LEFT;
                }

                f->etat = RUNNING;
                f->newEtat = TRUE;
                f->evoSprite = 0;
                f->cdAtt = 0;
                f->ut = 0;
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

                if(!(p->sounds & puissance(2,SOUND_ARC)))
                    p->sounds += puissance(2,SOUND_ARC);
            }else{
                (p->nbFrameAtk)++;
            }
        }else{
            if(p->sounds & puissance(2,SOUND_ARC))
                p->sounds -= puissance(2,SOUND_ARC);

            if(tryAtk && p->etat <= RUNNING){
                p->etat = ATTACKING;
                p->newEtat = TRUE;
                p->nbFrameAtk = 0;
                }
            }
        }
}

void transformation(personnage_t* p, salle_t* s){
  SDL_Texture * temp=NULL;
  int deltaytmp=0;
  if(p->inventaire[7]){
    if(p->etat != ATTACKING){
        if(p->forme == 'h'){
            p->forme = 'r';
            p->hitboxActuelle=1;
            if(persValid(p,s)){
              p->etat=IDLE;
              p->newEtat=TRUE;
              p->nbAnim[IDLE]=1;
              p->nbAnim[RUNNING]=5;
              p->nbAnim[JUMPING]=3;
              p->nbAnim[ATTACKING]=0;
              p->spriteActuel.x=0;
              p->spriteActuel.y=IDLE;
              p->spriteActuel.w=LARGEURSPRITERENARD;
              p->spriteActuel.h=HAUTEURSPRITERENARD;
              p->evoSprite=p->vitAnim[p->etat];
              temp=p->sprites;
              p->sprites=p->spritesR;
              p->spritesR=temp;
            }
            else{
              p->forme = 'h';
              p->hitboxActuelle=0;
            }
        }else if(p->forme == 'r'){
            p->forme = 'h';
            p->pos.y-=3;
            deltaytmp=p->delta.y;
            p->delta.y=TAILLEBLOC-1;
            p->hitboxActuelle=0;

            if(persValid(p,s)){
              p->etat=IDLE;
              p->newEtat=TRUE;
              p->nbAnim[IDLE]=1;
              p->nbAnim[RUNNING]=8;
              p->nbAnim[JUMPING]=8;
              p->nbAnim[ATTACKING]=3;
              p->spriteActuel.x=0;
              p->spriteActuel.y=IDLE;
              p->spriteActuel.w=LARGEURSPRITEPERS;
              p->spriteActuel.h=HAUTEURSPRITEPERS;
              p->evoSprite=p->vitAnim[p->etat];
              temp=p->sprites;
              p->sprites=p->spritesR;
              p->spritesR=temp;
            }
          else{
            p->forme = 'r';
            p->hitboxActuelle=1;
            p->pos.y+=3;
            p->delta.y=deltaytmp;
          }
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
    SDL_Rect pH;
    SDL_Rect porteH;

    if(strcmp(lPortes->type,"porte"))
        return NULL;

    porte_t porte;
    char* salle = NULL;
    int i = 1;

    enTete(lPortes);
    while(!horsListe(lPortes)){
        valeurElm(lPortes,&porte);
        porteH.x = porte.pos.x * TAILLEBLOC+ TAILLEBLOC/2;
        porteH.w = TAILLEBLOC/4;
        porteH.y = porte.pos.y * TAILLEBLOC;
        porteH.h = TAILLEBLOC;

        pH.w=p->hitbox[p->etat + p->hitboxActuelle*NBETATS].w;
        pH.h=p->hitbox[p->etat + p->hitboxActuelle*NBETATS].h;
        pH.y=p->pos.y*TAILLEBLOC + p->delta.y + p->hitbox[p->etat + p->hitboxActuelle*NBETATS].y;
        pH.x=p->pos.x*TAILLEBLOC + p->delta.x;
        if(p->direction){
          pH.x+=p->spriteActuel.w - p->hitbox[p->etat + p->hitboxActuelle*NBETATS].x - pH.w ;
        }
        else{
          pH.x+=p->hitbox[p->etat + p->hitboxActuelle*NBETATS].x;
        }

        if(SDL_HasIntersection(&(porteH),&(pH))){
            p->pos = porte.pos_arrivee;
            p->delta.x = 0;
            p->delta.y = TAILLEBLOC -1;
            p->apparition = porte.pos_arrivee;
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
  SDL_Rect mH;
  SDL_Rect bloc;
  bloc.w=TAILLEBLOC;
  bloc.h=TAILLEBLOC;

  for(int i=0; i<m->type->nbHitbox; i++){
    mH.w=m->type->hitbox[m->etat+ i*NBETATS].w;
    mH.h=m->type->hitbox[m->etat+ i*NBETATS].h;
    mH.y=m->pos.y*TAILLEBLOC + m->delta.y + m->type->hitbox[m->etat+ i*NBETATS].y;
    mH.x=m->pos.x*TAILLEBLOC + m->delta.x;
    if(m->direction){
      mH.x+=m->type->tailleSprite.largeur - m->type->hitbox[m->etat+ i*NBETATS].x - mH.w ;
    }
    else{
      mH.x+=m->type->hitbox[m->etat+ i*NBETATS].x;
    }

    if(mH.y + mH.h >= s->hauteur*TAILLEBLOC|| mH.x + mH.w >= s->largeur*TAILLEBLOC || mH.x < 0 || mH.y < 0)
        return TRUE;

    for(int i = 0; i < s->largeur; i++){
        for(int j = 0; j < s->hauteur; j++){
          if(s->mat[j][i]){
            bloc.x=i*bloc.w;
            bloc.y=j*bloc.h;
            if(SDL_HasIntersection(&mH, &bloc)){
              return TRUE;
            }
          }
        }
    }
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
/*static int verifChuteMonstre(monstre_t* m, salle_t* s){
    int leftM;
    int rightM;
    int bottomM;

    leftM = m->pos.x*TAILLEBLOC + m->delta.x + m->type->hitbox.x;
    rightM = leftM + m->type->hitbox.w;
    bottomM = m->pos.y*TAILLEBLOC + m->delta.y + m->type->hitbox.y + m->type->hitbox.h;

    leftM /= TAILLEBLOC;
    rightM = rightM/TAILLEBLOC;
    bottomM = bottomM/TAILLEBLOC - (bottomM%TAILLEBLOC ? 1 : 0);

    if(bottomM >= s->hauteur)
        return -1; //Le monstre est tombé dans le vide ?

    for(int i = leftM; i <= rightM; i++){
        if(!s->mat[bottomM][i])
            return TRUE;
    }

    return FALSE;
}*/

/**
 * \brief Gère le déplacement d'une entite (monstre)
 *
 * @param entite le pointeur vers la structure monstre à déplacer
 * @param salle le pointeur vers la structure salle où se trouve l'entite
*/
boolean_t depH(monstre_t* entite, salle_t* salle){
    int oldposx=entite->pos.x;
    int olddeltax=entite->delta.x;
    if(entite->pv){
        if(entite->direction){
            entite->delta.x += entite->type->vit_dep;
            if(entite->delta.x >= TAILLEBLOC){
                (entite->pos.x)++;
                entite->delta.x -= TAILLEBLOC;
            }

        }else{
            entite->delta.x -= entite->type->vit_dep;
            if(entite->delta.x < 0){
                (entite->pos.x)--;
                entite->delta.x += TAILLEBLOC;
            }

        }
        if(!hitB(entite,salle))
            return TRUE;
        else{
          entite->pos.x=oldposx;
          entite->delta.x=olddeltax;
          return FALSE;
        }
    }
    return FALSE;
}

boolean_t depV(monstre_t* entite, salle_t* salle, boolean_t chute){
    int oldposy = entite->pos.y;
    int olddeltay = entite->delta.y;
    if(entite->pv){
        entite->delta.y += entite->type->vit_dep*3;
        if(entite->delta.y >= TAILLEBLOC){
            (entite->pos.y)++;
            entite->delta.y -= TAILLEBLOC;
        }
        if(chute){
          if(!hitB(entite,salle))
            return TRUE;
          else{
            entite->pos.y=oldposy;
            entite->delta.y=olddeltay;
            return TRUE;
          }
        }
        else{
          if(hitB(entite,salle)){
            entite->pos.y=oldposy;
            entite->delta.y=olddeltay;
            return TRUE;
          }
          else{
            entite->pos.y=oldposy;
            entite->delta.y=olddeltay;
            return FALSE;
          }
        }
    }
    return FALSE;
}

static int inRange(monstre_t* m, personnage_t* perso, salle_t* salle, boolean_t checkUpside){
    SDL_Rect pH;
    SDL_Rect mH;
    SDL_Rect bloc;
    bloc.w=TAILLEBLOC;
    bloc.h=TAILLEBLOC;

    pH.w=perso->hitbox[perso->etat + perso->hitboxActuelle*NBETATS].w;
    pH.h=perso->hitbox[perso->etat + perso->hitboxActuelle*NBETATS].h;
    pH.y=perso->pos.y*TAILLEBLOC + perso->delta.y + perso->hitbox[perso->etat + perso->hitboxActuelle*NBETATS].y;
    pH.x=perso->pos.x*TAILLEBLOC + perso->delta.x;
    if(perso->direction){
      pH.x+=perso->spriteActuel.w - perso->hitbox[perso->etat + perso->hitboxActuelle*NBETATS].x - pH.w ;
    }
    else{
      pH.x+=perso->hitbox[perso->etat + perso->hitboxActuelle*NBETATS].x;
    }

    for(int i=0; i<m->type->nbHitbox; i++){
      mH.w=m->type->hitbox[m->etat+ i*NBETATS].w + m->type->radius;
      mH.h=m->type->hitbox[m->etat+ i*NBETATS].h;
      mH.y=m->pos.y*TAILLEBLOC + m->delta.y + m->type->hitbox[m->etat+ i*NBETATS].y;
      mH.x=m->pos.x*TAILLEBLOC + m->delta.x;
      if(!m->direction){
        mH.x+=m->type->tailleSprite.largeur - m->type->hitbox[m->etat+ i*NBETATS].x - mH.w;
      }
      else{
        mH.x+=m->type->hitbox[m->etat+ i*NBETATS].x;
      }
      if(!checkUpside && ((pH.y + pH.h) < (mH.y) || (pH.y) > (mH.y + mH.h)))
        return FALSE;

        for(int i = 0; i < salle->largeur; i++){
            for(int j = 0; j < salle->hauteur; j++){
              if(salle->mat[j][i]){
                bloc.x=i*bloc.w;
                bloc.y=j*bloc.h;
                if(SDL_HasIntersection(&mH, &bloc)){
                  if(!((m->direction && (bloc.x>pH.x)) || (!m->direction && (bloc.x<pH.x))))
                    if(SDL_HasIntersection(&mH, &pH))
                      return FALSE;
                }
              }
            }
          }
        if(SDL_HasIntersection(&mH, &pH))
          return TRUE;
      }
    return FALSE;
}

void compRecuperable(monstre_t* entite, personnage_t* perso, salle_t* salle){
    entite->etat = IDLE;
    if(hitP(entite,perso))
        recupElem(entite,perso);
}

void compCoeur(monstre_t* e, personnage_t* p, salle_t* s){
    if(hitP(e,p)){
        p->pv += e->type->degat;
        e->pv = 0;
        if(!(p->sounds & puissance(2,SOUND_ITEM)))
            p->sounds += puissance(2,SOUND_ITEM);
    }else{
        if(e->type->vit_dep == 1){
            e->delta.y += e->type->vit_dep;
            if(e->delta.y >= TAILLEBLOC){
                (e->pos.y)++;
                e->delta.y -= TAILLEBLOC;
            }
            if(hitB(e,s)){
                e->delta.y -= e->type->vit_dep;
                if(e->delta.y < 0){
                    e->delta.y += TAILLEBLOC;
                    (e->pos.y)--;
                }
            }
            e->type->vit_dep = 3;
        }else
            e->type->vit_dep--;
    }
}

void compFleches(monstre_t* entite, personnage_t* perso, salle_t* salle){
    monstre_t tmp;

    if(!depH(entite,salle)){
        entite->pv = 0;
        entite->etat = IDLE;
        entite->newEtat = TRUE;
    }

    enTete(salle->listeEntite);
    while(!horsListe(salle->listeEntite)){
        valeurElm(salle->listeEntite,&tmp);
        if(hitE(entite,&tmp)){
            if(tmp.type->comportement != compPortes){
              if(!(perso->sounds & puissance(2,SOUND_TOUCHE)))
                  perso->sounds += puissance(2,SOUND_TOUCHE);
                tmp.pv -= entite->type->degat;
                modifElm(salle->listeEntite,&tmp);
            }
            entite->pv = 0;
            entite->newEtat = TRUE;

        }
        suivant(salle->listeEntite);
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
            if(!(p->sounds & puissance(2,SOUND_DOOR)))
                p->sounds += puissance(2,SOUND_DOOR);
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
    if(dir && !perso->hit){
        perso->pv -= entite->type->degat;
        perso->kb = 1;
        perso->hit = TRUE;
        perso->etat = IDLE;

        if(dir==-1)
          perso->direction=RIGHT;
        else
          perso->direction=LEFT;

        entite->direction = perso->direction;

        if(!(perso->sounds & puissance(2,SOUND_TOUCHE)))
            perso->sounds += puissance(2,SOUND_TOUCHE);
    }else{
        entite->ut--;
        if(entite->ut <= 0){
            if(!(depH(entite,salle))){
                if(entite->direction)
                    entite->direction = LEFT;
                else
                    entite->direction = RIGHT;
            }
            else if(!(depV(entite,salle,FALSE))){
              if(entite->direction)
                  entite->direction = LEFT;
              else
                  entite->direction = RIGHT;
            }
            entite->ut = 2;
        }
    }
}

void compSerpentRose(monstre_t* entite, personnage_t* perso, salle_t* salle){
  SDL_Rect mH;
    int dir = hitP(entite,perso);
    if(dir && !perso->hit){
        perso->kb = 1;
        perso->hit = FALSE;
        perso->etat = IDLE;

        if(dir==-1)
          perso->direction=RIGHT;
        else
          perso->direction=LEFT;

        entite->direction = perso->direction;

    }

    if(entite->cdAtt > 0)
        entite->cdAtt--;

    if(!entite->cdAtt && (entite->etat == ATTACKING || inRange(entite,perso,salle,FALSE))){
        if(entite->etat != ATTACKING){
            entite->ut = 10;
            entite->etat = ATTACKING;
            entite->newEtat = TRUE;
            if(!(perso->sounds & puissance(2,SOUND_SERPENT)))
                perso->sounds += puissance(2,SOUND_SERPENT);
        }else{
            if(!(--(entite->ut))){
                monstre_t* f = malloc(sizeof(monstre_t));
                f->type = &typesMonstre[-VENIN - 1];

                f->pv = f->type->pv_base;

                mH.w=entite->type->hitbox[entite->etat].w;
                mH.h=entite->type->hitbox[entite->etat].h;
                mH.y=entite->pos.y*TAILLEBLOC + entite->delta.y + entite->type->hitbox[entite->etat].y;
                mH.x=entite->pos.x*TAILLEBLOC + entite->delta.x;
                if(entite->direction){
                  f->direction = RIGHT;
                  mH.x+=entite->type->tailleSprite.largeur - entite->type->hitbox[entite->etat].x - mH.w ;
                  f->delta.x=0;
                  f->delta.y=(mH.y + mH.h/2)%TAILLEBLOC;
                  f->pos.x=(mH.x + mH.w)/TAILLEBLOC;
                  f->pos.y=(mH.y + mH.h/2)/TAILLEBLOC -1;
                  f->delta.y+=2;
                  if(f->delta.y>=TAILLEBLOC) {
                    f->delta.y-=TAILLEBLOC;
                    f->pos.y++;
                  }
                }
                else{
                  f->direction = LEFT;
                  mH.x+=entite->type->hitbox[entite->etat].x;
                  f->delta.x=0;
                  f->delta.y=(mH.y + mH.h/2)%TAILLEBLOC;
                  f->pos.x=(mH.x)/TAILLEBLOC;
                  f->pos.y=(mH.y + mH.h/2)/TAILLEBLOC -1;
                  f->delta.y+=2;
                  if(f->delta.y>=TAILLEBLOC) {
                    f->delta.y-=TAILLEBLOC;
                    f->pos.y++;
                  }
                }

                f->etat = IDLE;

                f->spriteActuel.h = f->type->tailleSprite.hauteur;
                f->spriteActuel.w = f->type->tailleSprite.largeur;
                f->spriteActuel.x = 0;
                f->spriteActuel.y = f->etat * f->spriteActuel.h;

                enTete(salle->listeEntite);
                ajoutDroit(salle->listeEntite, f);

                entite->etat = RUNNING;
                entite->newEtat = TRUE;
                entite->cdAtt = entite->type->vit_att;
            }
        }
    }else{
        entite->ut--;
        if(entite->ut <= 0){
          if(!(depH(entite,salle))){
              if(entite->direction)
                  entite->direction = LEFT;
              else
                  entite->direction = RIGHT;
          }
          else if(!(depV(entite,salle,FALSE))){
            if(entite->direction)
                entite->direction = LEFT;
            else
                entite->direction = RIGHT;
          }
          entite->ut = 2;
        }
    }
}

void compSingeGrotte(monstre_t* entite, personnage_t* perso, salle_t* salle){
    int dir = hitP(entite,perso);
    int tmp;
    if(dir && !perso->hit){
        perso->pv -= entite->type->degat;
        perso->kb = 1;
        perso->etat = IDLE;
        perso->hit = TRUE;
        entite->newEtat = TRUE;

        if(dir==-1)
          perso->direction=RIGHT;
        else
          perso->direction=LEFT;

        entite->direction = perso->direction;

        if(!(perso->sounds & puissance(2,SOUND_TOUCHE)))
            perso->sounds += puissance(2,SOUND_TOUCHE);
    }
    if(perso->direction){
      tmp=perso->spriteActuel.w - perso->hitbox[perso->etat + perso->hitboxActuelle*NBETATS].x - perso->hitbox[perso->etat + perso->hitboxActuelle*NBETATS].w ;
    }
    else{
      tmp=perso->hitbox[perso->etat + perso->hitboxActuelle*NBETATS].x;
    }
    if(entite->pos.x * TAILLEBLOC + entite->delta.x + entite->type->hitbox[entite->etat].x> perso->pos.x * TAILLEBLOC + perso->delta.x + tmp)
        entite->direction = LEFT;
    else
        entite->direction = RIGHT;

    switch(entite->etat){
        case RUNNING:
            entite->etat = IDLE;
            break;
        case IDLE:
            if(inRange(entite,perso,salle,TRUE)){
                entite->etat = ATTACKING;
                entite->newEtat = TRUE;
                entite->ut = 10;
                if(!(perso->sounds & puissance(2,SOUND_SINGE)))
                    perso->sounds += puissance(2,SOUND_SINGE);
            }
            break;
        case JUMPING:
            entite->delta.y -= entite->type->vit_dep;
            if(entite->delta.y < 0){
                (entite->pos.y)--;
                entite->delta.y += TAILLEBLOC;
            }
            if(hitB(entite,salle)){
                entite->etat = FALLING;
                entite->newEtat = TRUE;
                (entite->pos.y)++;
                entite->delta.y -= TAILLEBLOC;
            }
            break;
        case ATTACKING:
            entite->ut--;
            if(!entite->ut){
                entite->etat = JUMPING;
                entite->newEtat = TRUE;
            }
            break;
        case FALLING:
            entite->delta.y += entite->type->vit_dep;
            if(entite->delta.y >= TAILLEBLOC){
                (entite->pos.y)++;
                entite->delta.y -= TAILLEBLOC;
            }
            if(hitB(entite,salle)){
                entite->etat = IDLE;
                entite->newEtat = TRUE;
                (entite->pos.y)--;
                entite->delta.y += TAILLEBLOC;
            }
            break;
        default:
            break;
    }
}

void compVenin(monstre_t* entite, personnage_t* perso, salle_t* salle){
    int dir = hitP(entite,perso);
    if(dir && !perso->hit){
        perso->pv -= entite->type->degat;
        perso->kb = 1;
        perso->hit = TRUE;
        perso->etat = IDLE;
        entite->pv = 0;
        perso->direction = dir > 0 ? 0 : 1;
        if(!(perso->sounds & puissance(2,SOUND_TOUCHE)))
            perso->sounds += puissance(2,SOUND_TOUCHE);
    }else
        if(!depH(entite,salle))
            entite->pv = 0;
}

void compVersGeant(monstre_t* entite, personnage_t* perso, salle_t* salle){

}

void compVifplume(monstre_t* entite, personnage_t* perso, salle_t* salle){
    int dir = hitP(entite,perso);
    int tmp;
    if(dir && !perso->hit){
        perso->pv -= entite->type->degat;
        perso->kb = 1;
        perso->etat = IDLE;
        perso->hit = TRUE;
        entite->etat = IDLE;
        entite->ut = 0;
        entite->newEtat = TRUE;

        if(dir==-1)
          perso->direction=RIGHT;
        else
          perso->direction=LEFT;

        entite->direction = perso->direction;
        if(!(perso->sounds & puissance(2,SOUND_TOUCHE)))
            perso->sounds += puissance(2,SOUND_TOUCHE);
    }

    if(perso->direction){
      tmp=perso->spriteActuel.w - perso->hitbox[perso->etat + perso->hitboxActuelle*NBETATS].x - perso->hitbox[perso->etat + perso->hitboxActuelle*NBETATS].w ;
    }
    else{
      tmp=perso->hitbox[perso->etat + perso->hitboxActuelle*NBETATS].x;
    }
    if(entite->pos.x * TAILLEBLOC + entite->delta.x + entite->type->hitbox[entite->etat].x> perso->pos.x * TAILLEBLOC + perso->delta.x + tmp)
        entite->direction = LEFT;
    else
        entite->direction = RIGHT;

    switch(entite->etat){
        case RUNNING:
            if(entite->ut != 1){
                entite->etat = IDLE;
            }
            else{
              if(!(depH(entite,salle))){
                entite->etat=IDLE;
              }
              else if(!(depV(entite,salle,FALSE))){
                entite->etat=IDLE;
              }
            }
            break;
        case IDLE:
            if((--(entite->cdAtt)) <= 0 && inRange(entite,perso,salle,FALSE)){
                entite->etat = ATTACKING;
                entite->newEtat = TRUE;
                entite->ut = 20;
                entite->cdAtt = entite->type->vit_att;
                if(!(perso->sounds & puissance(2,SOUND_VIFPLUME)))
                    perso->sounds += puissance(2,SOUND_VIFPLUME);
            }
            break;
        case ATTACKING:
            entite->ut--;
            if(!entite->ut){
                entite->etat = RUNNING;
                entite->newEtat = TRUE;
                entite->ut = 1;
            }
            break;
        default:
            break;
    }
}

static void creerCoeur(monstre_t* m, salle_t* s){
  SDL_Rect mH;
  monstre_t* c = malloc(sizeof(monstre_t));
  c->type = &typesMonstre[-COEUR - 1];
  c->pv = c->type->pv_base;

  mH.w=m->type->hitbox[m->etat].w;
  mH.h=m->type->hitbox[m->etat].h;
  mH.y=m->pos.y*TAILLEBLOC + m->delta.y + m->type->hitbox[m->etat].y;
  mH.x=m->pos.x*TAILLEBLOC + m->delta.x;
  c->direction = LEFT;
  mH.x+=m->type->hitbox[m->etat].x;
  c->delta.x=(mH.x+mH.w/2)%TAILLEBLOC;
  c->delta.y=m->delta.y;
  c->pos.x=(mH.x+mH.w/2)/TAILLEBLOC;
  c->pos.y=m->pos.y;

  c->etat = IDLE;
  c->newEtat = TRUE;
  c->evoSprite = 0;
  c->cdAtt = 0;
  c->ut = 0;

  c->spriteActuel.h = c->type->tailleSprite.hauteur;
  c->spriteActuel.w = c->type->tailleSprite.largeur;
  c->spriteActuel.x = 0;
  c->spriteActuel.y = c->etat * c->spriteActuel.h;

  enTete(s->listeEntite);
  ajoutDroit(s->listeEntite, c);
}

void evolution(personnage_t* p, salle_t* s){
    int r;

    p->sounds = 0 + (p->sounds & puissance(2,SOUND_ARC));

    if(!p->kb){
        if(p->inv)
            (p->inv)--;
        else
            p->hit = FALSE;
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

            if(!strcmp(e.type->nom,"serpent rose")){
                monstre_t tmp;
                valeurElm(s->listeEntite,&tmp);
                if(strcmp(tmp.type->nom,"serpent rose")){
                    suivant(s->listeEntite);
                    (s->listeEntite->indTmp)++;
                }
            }

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
