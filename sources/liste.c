#include <stdlib.h>
#include <string.h>
#include "../headers/structs.h"

/**
 * \file liste.c
 * \brief Librairie de fonctions pour des listes de \a monstre_t ou \a porte_t
 * \author Thomas DIDIER L2 Info Le Mans
 * \version 2.0
 * \date 13/02/2020
*/

/**
 * \brief Initialise une liste qui vient d'être créé
 *
 * @param maListe pointeur vers la liste à initialiser
 * @param type type de la liste (valeurs disponibles: \e monstre et \e porte )
*/
static void initListe(liste_t *maListe, char *type){
    maListe->drapeau = malloc(sizeof (elemListe_t));
    maListe->ec = maListe->drapeau->pred = maListe->drapeau->succ = maListe->drapeau;
    maListe->type = malloc(sizeof(char) * (strlen(type) + 1));
    strcpy(maListe->type, type);
}

/**
 * \brief Creer une liste du type demandé ( \a monstre_t ou \a porte_t )
 *
 * @param type type de la liste (valeurs disponibles: \e monstre et \e porte )
 * @return adresse de la liste créée
*/
liste_t* creerListe(char* type){
    liste_t* maListe = malloc(sizeof(elemListe_t*));
    initListe(maListe, type);
    return maListe;
}

/**
 * \brief Indique si la liste est vide ou non
 *
 * @param maListe liste à traiter
 * @return 1 si la liste est vide, 0 sinon
*/
int listeVide(liste_t *maListe){
    return maListe->drapeau->succ == maListe->drapeau;
}

/**
 * \brief Indique si l'élément courant est hors liste
 *
 * @param maListe liste à vérifier
 * @return 1 si l'élément est hors liste, 0 sinon
*/
int horsListe(liste_t *maListe){
    return maListe->ec == maListe->drapeau;
}

/**
 * \brief Met en tête de liste
 *
 * @param maListe liste dont il faut se mettre en tête
*/
void enTete(liste_t *maListe){
    if(!listeVide(maListe))
        maListe->ec = maListe->drapeau->succ;
}

/**
 * \brief Met en queue de liste
 *
 * @param maListe liste dont il faut se mettre en queue
*/
void enQueue(liste_t *maListe){
    if(!listeVide(maListe))
        maListe->ec = maListe->drapeau->pred;
}

/**
 * \brief Passe à l'élément suivant de la liste
 *
 * @param maListe liste dont il faut passer à l'élément suivant
*/
void suivant(liste_t *maListe){
    if(!horsListe(maListe))
        maListe->ec = maListe->ec->succ;
}

/**
 * \brief Revient à l'élément précédent de la liste
 *
 * @param maListe liste dont il faut revenir à l'élément précédent
*/
void precedent(liste_t *maListe){
    if(!horsListe(maListe))
        maListe->ec = maListe->ec->pred;
}

/**
 * \brief Récupère la valeur de l'élément courant de la liste
 * \details L'élément reçu est une copie de l'élément courant, le modifier ou le supprimer n'impacte pas la liste
 *
 * @param maListe liste dont il faut récupérer l'élément
 * @param v adresse vers l'élément de récéption
 * @return le type de l'élément reçu ( \e monstre si il est de type \a monstre_t et \e porte si il est de type \a porte_t )
*/
char* valeurElm(liste_t *maListe, void *v){
    if(!horsListe(maListe)){
        if(!strcmp(maListe->type,"monstre")){
            *(monstre_t *)v = *(monstre_t *)(maListe->ec->entite);
        }else{
            *(porte_t *)v = *(porte_t *)(maListe->ec->entite);
        }
        return maListe->type;
    }

    return "\0";
}

/**
 * \brief Modifie l'élément courant de la liste
 * \details Copie l'élément donné dans l'élément courant: l'élément donné n'est pas mis dans la liste et la zone mémoire allouée (si il y a), doit être libéré plus tard et peut être réutilisé
 *
 * @param maListe liste dont il faut modifier l'élément courant
 * @param v adresse vers l'élément à copier
*/
void modifElm(liste_t *maListe, void *v){
    if(!horsListe(maListe)){
        if(!strcmp(maListe->type,"monstre")){
            *(monstre_t *)(maListe->ec->entite) = *(monstre_t *)v;
        }
        else{
            *(porte_t *)(maListe->ec->entite) = *(porte_t *)v;
        }
    }
}

/**
 * \brief Ote l'élément courant de la liste
 *
 * @param maListe liste dont il faut oter l'élément courant
*/
void oterElm(liste_t *maListe, void (*delete)(void**)){
    if(!horsListe(maListe)){
        maListe->ec->pred->succ = maListe->ec->succ;
        elemListe_t *tmp = maListe->ec->succ->pred = maListe->ec->pred;
        delete(&(maListe->ec->entite));
        free(maListe->ec);
        maListe->ec = tmp;
    }
}

/**
 * \brief Ajoute un élément dans la liste à droite de l'élément courrant
 * \details L'élément est ajouté par copie de son adresse. Tout élément ajouté doit impérativement être alloué dans le tas.
 * \warning Tout élément donné pour ajout \b DOIT \b IMPERATIVEMENT être alloué dans le tas
 *
 * @param maListe liste dont il faut ajouter l'élément
 * @param v adresse vers l'élément à ajouter
*/
void ajoutDroit(liste_t *maListe, void *v){
    if(listeVide(maListe) || !horsListe(maListe)){
        elemListe_t *nouv = malloc(sizeof (*nouv));
        nouv->entite = v;
        nouv->succ = maListe->ec->succ;
        maListe->ec->succ = nouv;
        nouv->pred = maListe->ec;
        nouv->succ->pred = nouv;
        maListe->ec = nouv;
    }
}

/**
 * \brief Ajoute un élément dans la liste à gauche de l'élément courrant
 * \details L'élément est ajouté par copie de son adresse. Tout élément ajouté doit impérativement être alloué dans le tas.
 * \warning Tout élément donné pour ajout \b DOIT \b IMPERATIVEMENT être alloué dans le tas
 *
 * @param maListe liste dont il faut ajouter l'élément
 * @param v adresse vers l'élément à ajouter
*/
void ajoutGauche(liste_t *maListe, void *v){
    if(listeVide(maListe) || !horsListe(maListe)){
        elemListe_t *nouv = malloc(sizeof (*nouv));
        nouv->entite = v;
        nouv->pred = maListe->ec->pred;
        maListe->ec->pred = nouv;
        nouv->succ = maListe->ec;
        nouv->pred->succ = nouv;
        maListe->ec = nouv;
    }
}

/**
 * \brief Supprime la liste donné
 * \details La fonction met aussi la valeur du pointeur de la liste à NULL
 *
 * @param maListe adresse vers la liste à supprimer
*/
void supListe(liste_t **maListe, void (*delete)(void**)){
    enQueue(*maListe);
    while(!listeVide(*maListe))
        oterElm(*maListe,delete);
    free((*maListe)->type);
    free((*maListe)->drapeau);
    free(*maListe);
    (*maListe) = NULL;
}

static void delPorte(void** p){
    free((*p)->salleSuivante);
    free((*p)->listeSprites);
    free(*p);
    *p = NULL;
}

void supPorte(porte_t** p){
    delPorte(p);
}

static void delMonstre(void** m){
    free(*m);
    *m = NULL;
}

void supMonstre(monstre_t** m){
    delMonstre(m);
}