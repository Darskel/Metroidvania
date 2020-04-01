/**
 * \file liste.h
 * \brief Header pour la librairie de fonctions pour des listes de \a monstre_t ou \a porte_t
 * \author Thomas DIDIER L2 Info Le Mans
 * \version 2.0
 * \date 13/02/2020
*/

#ifndef LISTE_INCLUDED
#define LISTE_INCLUDED

/**
 * \struct elemListe_s
 * \brief Un élément d'une liste (pouvant aussi être le drapeau de la liste)
*/
typedef struct elemListe_s{
    void *entite; /**< Objet de la liste */
    struct elemListe_s *pred; /**< Element précédent dans la liste */
    struct elemListe_s *succ; /**< Element suivant dans la liste */
} elemListe_t;

/**
 * \struct liste_s
 * \brief Une liste d'élément inconnu dans le type est donné en chaine de caractère
*/
typedef struct liste_s{
    char* type; /**< Nom du type de l'objet présent dans la liste (monstre ou porte) */
    elemListe_t *drapeau; /**< Drapeau de la liste */
    elemListe_t *ec; /**< Element courant de la liste */
} liste_t;

liste_t* creerListe(char* type);
int listeVide(liste_t *maListe);
int horsListe(liste_t *maListe);
void enTete(liste_t *maListe);
void enQueue(liste_t *maListe);
void suivant(liste_t *maListe);
void precedent(liste_t *maListe);
char* valeurElm(liste_t *maListe, void *v);
void modifElm(liste_t *maListe, void *v);
void oterElm(liste_t *maListe, void (*delete)(void**));
void ajoutDroit(liste_t *maListe, void *v);
void ajoutGauche(liste_t *maListe, void *v);
void supListe(liste_t **maListe, void (*delete)(void**));

void supPorte(porte_t**);
void supMonstre(monstre_t**);

#endif
