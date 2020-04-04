#include <stdio.h>
#include "liste.h"

#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#endif

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

#define NOM_JEU "Diskosieni"

#define PLAYERSPRITESPATH "./sprites/entite/joueur/sprite_joueur_hitbox.png"
#define TILESETPATH "./sprites/bloc/tileset.png"

#define TAILLEBLOC 8
#define HAUTEURPORTE 36
#define LARGEURPORTE 13
#define HAUTEURSPRITEPERS 33
#define LARGEURSPRITEPERS 25
#define LARGEURSPRITEPERSATTACK 30

#define HAUTEURHITBOXPERS 32
#define LARGEURHITBOXPERS 14
#define OFFSETHITBOX 6

#define VITDEPPERS 1
#define VITATTACKPERS 1

#define EVOSPRITES 3

#define ZONEMORTE 5000

#define NIVEAUTXT "salle_debut.txt"
#define NIVEAUBG "./sprites/salles/salle_debut.png"
#define DIRBG "./sprites/salles/"




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


/**
 * \struct taille_t
 * \brief Taille avec hauteur et largeur
*/
typedef struct taille_s{
  int hauteur;/**< Hauteur*/
  int largeur;/**< Largeur*/
}taille_t;

/**
 * \struct position_s
 * \brief Une position représentée par deux valeurs entières
*/
typedef struct position_s {
    int x; /**< Position en hauteur*/
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
    SDL_Texture * background; /**< Image d'arrière-plan de la salle */
    SDL_Texture * tileset; /**< Image qui contient tout les sprites de la tileset associée à la salle */
    int spriteActuel; /**< Indice du sprite à afficher */
    liste_t * listePorte; /**< Liste des portes de la salle */
} salle_t;

/**
 * \struct personnage_s
 * \brief Structure représentant le personnage
*/
typedef struct personnage_s{
    int pv; /**< PV(points de vie) actuel du personnage */
    int pv_max; /**< PV max du personnage */
    int inv; /**< Entier qui permet de décompter l'invulnérabilité du joueur */
    int nbPxSaut; /**< Entier qui indique le nombre de pixel qu'il a fait lors de son saut (ou deuxième saut) */
    int nbSaut; /**< Entier qui indique le nombre de saut que le personnage à fait avant de retoucher le sol */
    int direction; /*Vaut 1 si le personnage va à droite et 0 si il va à gauche */
    int vit_dep; /**< Vitesse de déplacement du personnage (pixel par tick) */
    int vit_att; /**< Vitesse d'attaque du personnage (en nombre de frame) */
    position_t pos; /**< Position actuel du personnage (position entière en cases de matrice) */
    position_t delta; /**< Position en pixel à l'intérieur de la case de matrice */
    SDL_Texture * sprites; /**Pointeur vers la texture qui contient les sprites du personnage */
    SDL_Rect spriteActuel; /**< Indice du sprite actuel en x et y dans la texture */
    taille_t hitbox; /**< Taille de la hitbox du personnage en pixel */
    int posxhitbox; /**< Position horizontale en pixel de la hitbox dans la salle */
    etat_t etat; /**< Etat du personnage (idle/running/jumping/attacking/falling) */
    boolean_t newEtat; /**< Booléen qui signifie qu'un changement d'état vient de s'effectuer */
    int evoSprite; /**< Entier qui décrémente, changement de sprite quand vaut 0 */
    int * nbAnim; /**< Tableau qui contient le nombre de sprites d'animation pour chaque action du personage */
    char forme; /**< Forme du personnage H = humain, F = renard */
    int inventaire[TAILLE_INVENTAIRE]; /**<Tableau qui contient les informations sur l'inventaire actuel du personnage */
    char* nomObj[TAILLE_INVENTAIRE]; /**<Tableau qui contient les noms des objets de l'inventaire */
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
    char* path; /**< Chemin d'accès à l'image qui contient les sprites*/
    SDL_Texture * sprites; /**Pointeur vers la texture qui contient les sprites du monstre */
    int * nbAnim; /**< Tableau qui contient le nombre de sprites d'animation pour chaque action du monstre */
    int degat;
    taille_t hitbox; /**< Taille de la hitbox de monstre en cases */
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
    etat_t etat;
    SDL_Rect spriteActuel; /**< Indice du sprite actuel en x et y dans la texture */
    position_t pos; /**< Position actuel du personnage (position entière en cases de matrice) */
    position_t delta; /**< Position en pixel à l'intérieur de la case de matrice */
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
    SDL_Texture * sprites; /**Pointeur vers la texture qui contient les sprites de la porte */
    int spriteActuel; /**< Indice du sprite à afficher */
} porte_t;


#endif
