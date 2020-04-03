#include <stdio.h>
#include "liste.h"

#ifndef SYSTEM_INCLUDED
#define SYSTEM_INCLUDED

    #if defined(_WIN32)
        #define PAUSE system("PAUSE");
        #define CLEAR system("CLS");
    #elif defined(__unix)
        #define PAUSE system("read -rsp $'Press any key to continue...\n' -n 1"); //Peut ne pas fonctionner parfois (PC de l'université par exemple)
        #define CLEAR system("clear");
    #else
        #define PAUSE
        #define CLEAR
    #endif

#endif

/**
 * \file structs.h
 * \brief Ensemble de structures utilisées dans le projets
 * \author Thomas DIDIER L2 Info Le Mans
 * \version 1.0
 * \date 13/02/2020
*/

#ifndef STRUCTS_INCLUDED
#define STRUCTS_INCLUDED

#define LEFT 0
#define RIGHT 1
#define TAILLE_INVENTAIRE 8

/*
    INVENTAIRE:
        objet n°1 : clé rouillé
        objet n°2 : clé rouge
        objet n°3 : clé verte
        objet n°4 : fléche de feu
        objet n°5 : clé bleu
        objet n°6 : double saut
        objet n°7 : renard
        objet n°8 : champignon
    >
        champignon
        cle bleue
        cle rouille
        cle rouge
        cle verte
        double saut
        fleche de feu
        renard

*/

typedef struct taille_s{
  int hauteur;
  int largeur;
}taille_t;

/**
 * \struct position_s
 * \brief Une position représentée par deux valeurs entières
*/
typedef struct position_s {
    int x; /**< Position en hauteur */
    int y; /**< Position en longueur*/
} position_t;

/**
 * \enum boulean_e
 * \brief Permet d'utiliser \e TRUE et \e FALSE
*/
typedef enum boulean_e{
    FALSE, /**< Valeur de FALSE: 0 */
    TRUE /**< Valeur de TRUE: 1 */
} boolean_t;

/**
 * \enum indSprite_e
 * \brief Nom des indices des sprites
*/
typedef enum indSprite_e{
    SU, /**< Sprite de non déplacement (Stand-Up) */
    DEP1, /**< Sprite de déplacement n°1 */
    DEP2, /**< Sprite de déplacement n°2 */
    DEP3, /**< Sprite de déplacement n°3 */
    DEP4, /**< Sprite de déplacement n°4 */
    DEP5, /**< Sprite de déplacement n°5 */
    DEP6, /**< Sprite de déplacement n°6 */
    DEP7, /**< Sprite de déplacement n°7 */
    DEP8, /**< Sprite de déplacement n°8 */
    JP1, /**< Sprite de saut (Jump) n°1 */
    JP2, /**< Sprite de saut (Jump) n°2 */
    JP3, /**< Sprite de saut (Jump) n°3 */
    JP4, /**< Sprite de saut (Jump) n°4 */
    JP5, /**< Sprite de saut (Jump) n°5 */
    JP6, /**< Sprite de saut (Jump) n°6 */
    JP7, /**< Sprite de saut (Jump) n°7 */
    JP8 /**< Sprite de saut (Jump) n°8 */
} indSpritePer_t;

typedef enum etat_e{
    IDLE,
    RUNNING,
    JUMPING,
    ATTACKING,
    FALLING
} etat_t;



/**
 * \struct salle_s
 * \brief Structure représentant les salles
*/
typedef struct salle_s{
    char * nomFichier; /**< Nom du fichier qui a généré la salle (utile pour les sauvegardes) */

    int hauteur; /**< Hauteur de la salle en blocks */
    int largeur; /**< Longueur de la salle en blocks */
    int ** mat; /**< Matrice de positionnement des objets */

    void*** sprites; /**< Indice du sprite à afficher */

    liste_t * listePorte;
} salle_t;

/**
 * \struct personnage_s
 * \brief Structure représentant le personnage
*/
typedef struct personnage_s{
    int pv; /**< PV(points de vie) actuel du personnage */
    int pv_max; /**< PV max du personnage */
    int vit_dep; /**< Vitesse de déplacement du personnage (pixel par tick) */
    int vit_att; /**< Vitesse d'attaque du personnage (en nombre de frame) */
    position_t pos; /**< Position actuel du personnage (position entière) */
    position_t delta; /**< Différence de position à ajouter à la position entière */

    int inv;

    #ifdef SDL_h_
    SDL_Surface ** sprites; /**Pointeur vers le tableau de sprites du personnage */
    #else
    void ** sprites; /**Pointeur vers le tableau de sprites du personnage */
    #endif

    indSpritePer_t spriteActuel; /**< Indice du sprite à afficher */
    int nb_sprites;/**< Nombre de sprites du personnage (sans orientation)*/
    char forme; /**< Forme du personnage H = humain, F = renard */
    taille_t hitbox;
    etat_t etat;
    int inventaire[TAILLE_INVENTAIRE];
    char* nomObj[TAILLE_INVENTAIRE];

} personnage_t;

/**
 * \struct type_monstre_s
 * \brief Structure représentant le type d'un monstre
*/
typedef struct type_monstre_s{
    int pv_base; /**< PV de base du monstre */
    int vit_dep; /**< Vitesse de déplacement du monstre (facteur/indicateur) */
    int vit_att; /**< Vitesse d'attaque du monstre (en nombre de frame) */

    char* nom;
    char* sprites; /**< Chemin d'accès aux sprites qui seront utilisés*/
    int nb_sprites;/**< Nombre de sprites du monstre (sans orientation)*/
    taille_t hitbox; /**< Largeur du monstre en unité de case (taille d'une case) */
    /**< Hauteur du monstre en unité de case (taille d'une case) */
    int degat;

    boolean_t passeEntites; /**< Indique si le monstre peut passer à travers les entités (autres monstres/joueur) */
    boolean_t passeBlocs; /**< Indique si le monstre peut passer à travers les blocs */

    //comportement (pointeur sur fonction)
} type_monstre_t;

/**
 * \struct monstre_s
 * \brief Structure représentant un monstre
*/
typedef struct monstre_s{
    type_monstre_t * type; /**< Type de monstre */
    int pv; /**< PV actuels du monstre */
    int spritesActuel; /**< Indice du sprite à afficher */
    etat_t etat;
    position_t pos; /**< Postition actuelle du monstre (position entière) */
    position_t delta; /**< Différence de position à ajouter à la position entière */
    boolean_t direction; /**< Direction vers laquelle regarde le monstre (1: vers la gauche(LEFT), 0: vers la droite(RIGHT)) */

} monstre_t;

/**
 * \struct bloc_s
 * \brief Structure représentant un bloc
*/
typedef struct bloc_s{

    int id_sprite; /**< Indice du sprite du bloc*/
    position_t pos; /**< Position du bloc */

} bloc_t;

/**
 * \struct porte_s
 * \brief Structure représentant une porte
*/
typedef struct porte_s{
    position_t pos; /**< Postition actuelle de la porte */
    char* salleSuivante; /**< Nom de la salle suivante (nom de la salle) */
    position_t pos_arrivee; /**< Postition d'apparition dans la salle d'arrivée */
    char* listeSprites; /**< Chemin vers les sprites de la porte */
    int spritesActuel; /**< Indice du sprite à afficher */
} porte_t;

#endif
