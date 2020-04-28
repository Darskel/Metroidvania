#include <stdio.h>
#include "liste.h"

#ifndef SDL_H
#define SDL_H

    #if defined(_WIN32)
        #include <SDL2/SDL.h>
        #include <SDL2/SDL_ttf.h>
        #include <SDL2/SDL_image.h>
        //#include <SDL2/SDL_mixer.h>
    #else
        #include <SDL.h>
        #include <SDL_ttf.h>
        #include <SDL_image.h>
        //#include <SDL_mixer.h>
    #endif

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
 * \author Thomas DIDIER Marie-Nina MUNAR L2 Info Le Mans
 * \version 2.2
 * \date 07/04/2020
*/

#ifndef STRUCTS_INCLUDED
#define STRUCTS_INCLUDED

#define LEFT 0
#define RIGHT 1

#define TAILLE_INVENTAIRE 8
#define INVENTAIRESIZE 10
#define INVENTAIREPATH "./sprites/inventaire/tileset.png"
#define INVENTAIRETIME 60

#define NOM_JEU "Diskosieni"

//Path :
#define TAILLEPATHFICHIER 50
#define PLAYERSPRITESPATH "./sprites/entite/joueur/tileset.png"
#define TILESETPATH "./sprites/bloc/tileset.png"
#define DIRBG "./sprites/salles/"

//Hauteur et largeur en pixels :
#define TAILLEBLOC 8
#define HAUTEURPORTE 36
#define LARGEURPORTE 13
#define HAUTEURSPRITEPERS 33
#define LARGEURSPRITEPERS 25
#define LARGEURSPRITEPERSATTACK 30

#define HAUTEURHITBOXPERS 32
#define LARGEURHITBOXPERS 13

#define HAUTEURHITBOXREN 12
#define LARGEURHITBOXREN 24

#define OFFSETHITBOX 6 //Différence de x (en pixel) entre le début du sprite et le début de l'hitbox

//Vitesses en pixel par secondes :
#define VITDEPPERS 1
#define VITSAUTPERS 2
#define VITCHUTEPERS 1
#define VITATTACKPERS 30

#define JPCD 5 //Délai avant de pouvoir de nouveau sauter

#define NBPXSAUT 5*TAILLEBLOC //Taille d'un saut en pixel

#define EVOSPRITES 3 //Change le sprite seulement 1 frame sur 3
#define EVOSPRITESATTACK EVOSPRITES*4 //Pareil mais pour les sprites d'attaque

#define ZONEMORTE 5000 //Zone morte de la manette

//Fichier du niveau de départ :
#define NIVEAUTXT "salle_debut.txt"

#define OFFSETWINDOW 0.05 //5% de la taille de l'écran en moins (pour mode fenêtré)

#define FRAMEDELAY 17 //Correspond à du 59fps

#define TAILLEKONAMI 11 //NB d'inputs du Konami code
#define KONAMICODE "uuddlrlrbas"

#define NBBLOCSTILESET 58
#define NBLOCERREUR 49

#define TAILLEBGMENUW 1920
#define TAILLEBGMENUH 1080

#define COEURDROPRATE 10

#define PVMAX 3
#define TEMPINV 60
#define FREQCLIGN 2
#define INVUDELAY 6

#define FONTSIZE 25

/*
    INVENTAIRE:
        objet n°1 : clé rouillée
        objet n°2 : clé rouge
        objet n°3 : clé verte
        objet n°4 : fléche de feu
        objet n°5 : clé bleu
        objet n°6 : double saut
        objet n°7 : renard
        objet n°8 : discoshroom
    >
        cle bleue
        cle rouge
        cle rouillee
        cle verte
        discoshroom
        double saut
        huile
        renard

*/

/**
 * \enum idEnt_e
 * \brief Id des entités présentes dans les fichiers de salle
*/
typedef enum idEnt_e{
    VENIN = -24,    /**< venin = -24 */
    FLECHEFEU,      /**< une fleche de feu = -23 */
    FLECHE,         /**< une flèche = -22 */
    COEUR,          /**< un coeur = -21 */
    VIFPLUME,       /**< vifplume = -20 */
    VERSGEANT,      /**< vers géant = -19 */
    SINGEGROTTE,    /**< singe grotte = -18 */
    SERPENTVERT,    /**< serpent vert = -17 */
    SERPENTROSE,    /**< serpent rose = -16 */
    SERPENTBLEU,    /**< serpent bleu = -15 */
    ROIVP,          /**< roi vifplume = -14 */
    RENARD,         /**< renard = -13 */
    PORTEV,         /**< porte verte = -12 */
    PORTERL,        /**< porte rouillée = -11 */
    PORTERG,        /**< porte rouge = -10 */
    PORTEB,         /**< porte bleue = -9 */
    MURGLACE,       /**< mur de glace = -8 */
    FLACONHUILE,    /**< fleches de feu = -7 */
    DOUBLESAUT,     /**< double saut = -6 */
    DISCOSHROOM,    /**< discoshroom = -5 */
    CLEVERTE,       /**< clé verte = -4 */
    CLEROUILLE,     /**< clé rouillée = -3 */
    CLEROUGE,       /**< clé rouge = -2 */
    CLEBLEUE,       /**< clé bleue = -1 */
} idEnt_t;


/**
 * \struct taille_s
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
 * \enum boolean_e
 * \brief Permet d'utiliser \e TRUE et \e FALSE
*/
typedef enum boolean_e{
    FALSE, /**< Valeur de FALSE: 0 */
    TRUE /**< Valeur de TRUE: 1 */
} boolean_t;

/**
 * \enum etat_e
 * \brief Nom des différents états possibles du personnage (utiles pour l'animation de sprites)
*/
typedef enum etat_e{
    IDLE, /**< Etat immobile */
    RUNNING, /**< Etat course */
    JUMPING, /**< Etat saut */
    ATTACKING, /**< Etat attaque */
    FALLING, /**< Etat chute */
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
    SDL_Rect spriteActuel; /**< Sprite d'arrière plan à afficher */
    int nbsprites; /**< Nombre de sprites de l'arrière plan */
    int animDelay; /**< Delai avant le changement de sprites d'arrière plan (vitesse animation) */
    int etatanim; /**< Etat du delai de l'animation du bg (entre 0 et animDelay) */
    liste_t * listePorte; /**< Liste des portes de la salle */
    liste_t * listeEntite; /**< Liste des entités de la salle */
} salle_t;

/**
 * \struct personnage_s
 * \brief Structure représentant le personnage
*/
typedef struct personnage_s{
    int pv; /**< PV(points de vie) actuel du personnage */
    boolean_t hit; /**< Indique si le personnage a été touché lors de la frame actuelle */
    int pv_max; /**< PV max du personnage */
    boolean_t kb; /**< Indique si le joueur est en plein knockback */
    int inv; /**< Entier qui permet de décompter l'invulnérabilité du joueur */
    int clign; /**Entier pour le clignotemment */
    int nbPxSaut; /**< Entier qui indique le nombre de pixel qu'il a fait lors de son saut (ou deuxième saut) */
    int nbFrameAtk; /**< Entier qui indique le nombre de frames qui a eu lieu depuis le début de l'attaque */
    int nbSaut; /**< Entier qui indique le nombre de saut que le personnage à fait avant de retoucher le sol */
    int jpCd; /**< Entier qui indique le nombre de frames restantes avant de pouvoir resauter */
    int direction; /*Vaut 1 si le personnage va à droite et 0 si il va à gauche */
    int vit_dep; /**< Vitesse de déplacement du personnage (pixel par tick) */
    int vit_saut; /**< Vitesse de saut du personnage (pixel par tick) */
    int vit_chute; /**< Vitesse de chute du personnage (pixel par tick) */
    int vit_att; /**< Vitesse d'attaque du personnage (en nombre de frame) */
    position_t pos; /**< Position actuel du personnage (position entière en cases de matrice) */
    position_t delta; /**< Position en pixel à l'intérieur de la case de matrice */
    position_t apparition; /**< Position d'apparition (utilisé pour les sauvegardes) */
    SDL_Texture * sprites; /**Pointeur vers la texture qui contient les sprites du personnage */
    SDL_Rect spriteActuel; /**< Indice du sprite actuel en x et y dans la texture */
    taille_t hitbox; /**< Taille de la hitbox du personnage en pixel */
    etat_t etat; /**< Etat du personnage (idle/running/jumping/attacking/falling) */
    boolean_t newEtat; /**< Booléen qui signifie qu'un changement d'état vient de s'effectuer */
    boolean_t newItem; /**< Booléen qui signifie que l'on vient de récolter un item */
    int evoSprite; /**< Entier qui décrémente, changement de sprite quand vaut 0 */
    int * nbAnim; /**< Tableau qui contient le nombre de sprites d'animation pour chaque action du personage */
    char forme; /**< Forme du personnage H = humain, R = renard */
    int inventaire[TAILLE_INVENTAIRE]; /**<Tableau qui contient les informations sur l'inventaire actuel du personnage */
    SDL_Texture * inventaireTileset;
    char* nomObj[TAILLE_INVENTAIRE]; /**<Tableau qui contient les noms des objets de l'inventaire */
} personnage_t;

typedef struct monstre_s monstre_t;

/**
 * \struct type_monstre_s
 * \brief Structure représentant le type d'un monstre
*/
typedef struct type_monstre_s{
    int pv_base; /**< PV de base du monstre */
    int vit_dep; /**< Vitesse de déplacement du monstre (facteur/indicateur) */
    int vit_att; /**< Vitesse d'attaque du monstre (en nombre de frame) */
    char* nom;  /**< Nom de l'entité/monstre (permet de gérer l'inventaire lors du pick up) */
    char* path; /**< Chemin d'accès à l'image qui contient les sprites*/
    SDL_Texture * sprites; /**Pointeur vers la texture qui contient les sprites du monstre */
    int * nbAnim; /**< Tableau qui contient le nombre de sprites d'animation pour chaque action du monstre */
    int degat; /**< Nombre de dégâts faits par le monstre */
    taille_t hitbox; /**< Taille de la hitbox de monstre en pixel */
    taille_t tailleSprite; /**< Taille des sprites de monstre en pixel */
    boolean_t passeEntites; /**< Indique si le monstre peut passer à travers les entités (autres monstres/joueur) */
    boolean_t passeBlocs; /**< Indique si le monstre peut passer à travers les blocs */
    int vitesseAnim; /**< Vitesse de l'animation des sprites (plus ce nombre est grand, plus la vitesse est lente (sert à evoSprite comme valeur avant décrémentation)) */
    //comportement (pointeur sur fonction)
    void (*comportement)(monstre_t* entite, personnage_t* perso, salle_t* salle);
} type_monstre_t;

/**
 * \struct monstre_s
 * \brief Structure représentant un monstre
*/
struct monstre_s{
    type_monstre_t * type; /**< Type de monstre */
    int pv; /**< PV actuels du monstre */
    etat_t etat; /**< Etat actuel du monstre */
    boolean_t newEtat; /**< Booléen qui signifie qu'un changement d'état vient de s'effectuer */
    SDL_Rect spriteActuel; /**< Rectangle qui correspond à la taille du sprite actuel dans la texture */
    position_t pos; /**< Position actuel du personnage (position entière en cases de matrice) */
    position_t delta; /**< Position en pixel à l'intérieur de la case de matrice */
    boolean_t direction; /**< Direction vers laquelle regarde le monstre (1: vers la gauche(LEFT), 0: vers la droite(RIGHT)) */
    int evoSprite; /**< Entier qui décrémente, changement de sprite quand vaut 0 */
    int cdAtt; /**< Variable indiquant le temps restant avant la prochaine attaque */
    int ut; /**< Variable utilitaire à fonction variable permetttant une info entière ou bouléenne sur un monstre: norme 0 = valeur de base ou rien */
};

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

struct menu_s;

/**
 * \enum idEnt_e
 * \brief Id des entités présentes dans les fichiers de salle
*/
typedef enum boutonetat_e{
  RELAXED,
  HIGHLIGHTED,
  PRESSED
} boutonetat_t;


/**
 * \struct menu_bouton_s
 * \brief Structure représentant un bouton d'un menu
*/
typedef struct menu_bouton_s{
  int id;
  char * etiquette;
  struct menu_s * parent;
  boutonetat_t etat;
  SDL_Texture ** texture;
  SDL_Rect emplacement;
} menu_bouton_t;

/**
 * \struct menu_texte_s
 * \brief Structure représentant un texte dans un menu
*/
typedef struct menu_texte_s{
  int id;
  char * etiquette;
  struct menu_s * parent;
  SDL_Texture * texture;
  SDL_Rect emplacement;
} menu_texte_t;

/**
 * \struct menu_s
 * \brief Structure représentant un menu
*/
typedef struct menu_s{
  char * etiquette;
  menu_bouton_t * tabBoutons;
  int nbBoutons;
  SDL_Texture * fond;
  menu_texte_t * tabTextes;
  int nbTextes;
} menu_t;

#endif
