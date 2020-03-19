#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/structs.h"

/**
 * \file testListe.c
 * \brief Programme permettant de tester la librairie de liste
 * \author Thomas DIDIER L2 Info Le Mans
 * \version 2.0
 * \date 13/02/2020
*/

/**
 * \brief Test si les éléments sont dans l'ordre
 * \details Ce test est pour une liste de 2 éléments dont le premier contient 1 dans spritesActuel et le second contient \a n dans spritesActuel
 * 
 * @param l liste à tester
 * @param n valeur de spritesActuel dans le second élément de la liste
 * @return 1 si le test est un succès, 0 sinon
*/
int testElem(liste_t* l, int n){
    porte_t p; /**< structure temporaire pour tester les valeurs */
    monstre_t m; /**< structure temporaire pour tester les valeurs */
    enTete(l);
    printf("Test de mise en tete: %s\n", l->ec == l->drapeau->succ ? "OK" : "erreur");

    //traitement monstre
    if(!strcmp(l->type,"monstre")){
        valeurElm(l,&m);
        if(m.spritesActuel != 2)
            return 0;
        suivant(l);
        valeurElm(l,&m);
        return m.spritesActuel == n;
    }

    //traitement porte
    valeurElm(l,&p);
    if(p.spritesActuel != 2)
        return 0;
    suivant(l);
    valeurElm(l,&p);
    return p.spritesActuel == n;
}

int main(){
    liste_t* l = NULL; /**< liste principale */
    porte_t* p = NULL; /**< structure pour importer dans la liste */
    monstre_t* m = NULL; /**< structure pour importer dans la liste */
    char test[2][10] = {"porte", "monstre"}; /**< Tableau pour faire le traitement sur les deux types de liste */

    for(int i = 0; i < 2; i++){
        //Création
        printf("   --- TESTS POUR %s ---   \n", test[i]);
        printf("Etat de la liste avant creation: %p\n", l);
        l = creerListe(test[i]);
        printf("Test de creation de la liste: %p\n", l ? "OK": "erreur");

        if(!l)
            return 1;

        printf("Etat du drapeau apres initialisation: %p\n", l->drapeau);
        printf("Etat de l'element courant apres initialisation: %p\n", l->ec);

        printf("Test liste vide: %s\n", listeVide(l) ? "OK" : "erreur");

        //Remplissage
        if(i){
            m = malloc(sizeof(*m));
            m->spritesActuel = 1;
            ajoutDroit(l,m);
            m = malloc(sizeof(*m));
            m->spritesActuel = 2;
            ajoutGauche(l,m);
        }else{
            p = malloc(sizeof(*p));
            p->spritesActuel = 1;
            ajoutDroit(l,p);
            p = malloc(sizeof(*p));
            p->spritesActuel = 2;
            ajoutGauche(l,p);
        }

        printf("Test liste vide apres ajout d'elements: %s\n", listeVide(l) ? "erreur" : "OK");

        printf("Test de placement des elements: %s\n", testElem(l,1) ? "OK" : "erreur");


        //Modification
        enTete(l);
        suivant(l);
        if(i){
            m = malloc(sizeof(*m));
            m->spritesActuel = 3;
            modifElm(l,m);
            free(m);
            m = NULL;            
        }else{
            p = malloc(sizeof(*p));
            p->spritesActuel = 3;
            modifElm(l,p);
            free(p);
            p = NULL;
        }

        printf("Test de modification d'un element: %s\n", testElem(l,3) ? "OK" : "erreur");

        //Suppression
        supListe(&l, i ? supMonstre : supPorte);
        printf("Etat de la liste apres suppression: %s\n", l ? "erreur" : "OK");
    }

    return 0;

}