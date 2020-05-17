#include <stdio.h>
#include "liste.h"

#ifndef SDL_H
#define SDL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#endif

/**
 * \file structs.h
 * \brief Ensemble de structures utilisées dans le projets
 * \author  Marie-Nina MUNAR & Thomas DIDIER L2 Info Le Mans
 * \version 3.0
 * \date 17/05/2020
*/

#ifndef STRUCTS_INCLUDED
#define STRUCTS_INCLUDED

#define LEFT 0
#define RIGHT 1

#define TAILLE_INVENTAIRE 8 /** Nombre d'item dans l'inventaire du joueur */
#define INVENTAIRESIZE 10 /** Taille de l'inventaire (hauteur) en pixel */
#define INVENTAIREPATH "./sprites/inventaire/tileset.png"
#define INVENTAIRETIME 60 /**Temps d'affichage de l'inventaire après arrêt d'appui du bouton */

#define VOLUMEBOXSIZE 28 /**Taille du rectangle contenant le volume (en pixels) */
#define VOLUMETIME 60 /**Temps d'affichage du volume après arrêt d'appui du bouton */

#define NOM_JEU "Diskosieni"

//Path :
#define TAILLEPATHFICHIER 75 /**Taille maximale du path de chaque fichier*/
#define PLAYERSPRITESPATH "./sprites/entite/joueur/tileset.png"
#define RENARDSPRITESPATH "./sprites/entite/renard/tileset.png"
#define TILESETPATH "./sprites/bloc/tileset.png"
#define DIRBG "./sprites/salles/"

//Hauteur et largeur en pixels :
#define TAILLEBLOC 8 /**Taille en pixel des blocs*/
#define HAUTEURPORTE 36 /**Hauteur en pixel d'une porte*/
#define LARGEURPORTE 13 /**Largeur en pixel d'une porte*/

#define LARGEURSPRITERENARD 24 /**Largeur en pixel du renard*/
#define HAUTEURSPRITERENARD 13 /**Hauteur en pixel du renard*/

#define HAUTEURSPRITEPERS 33 /**Hauteur en pixel du personnage*/
#define LARGEURSPRITEPERS 25 /**Largeur en pixel du personnage*/

#define LARGEURSPRITEPERSATTACK 30 /**Largeur en pixel du personnage*/

#define HAUTEURHITBOXPERS 32 /**Hauteur en pixel de la hitbox du personnage*/
#define LARGEURHITBOXPERS 13 /**Largeur en pixel de la hitbox du personnage*/

#define HAUTEURHITBOXREN 12 /**Hauteur en pixel de la hitbox du renard*/
#define LARGEURHITBOXREN 24 /**Largeur en pixel de la hitbox du renard*/

#define OFFSETHITBOXH 6 /**Différence de x (en pixel) entre le début du sprite et le début de l'hitbox */
#define OFFSETHITBOXR 0 /**Différence de x (en pixel) entre le début du sprite et le début de l'hitbox du renard */

//Vitesses en pixel par secondes :
#define VITDEPPERS 1 /**Vitesse de déplacement du personnage en pixel par tick*/
#define VITSAUTPERS 2 /**Vitesse de saut du personnage en pixel par tick*/
#define VITCHUTEPERS 1 /**Vitesse de chute du personnage en pixel par tick*/
#define VITATTACKPERS 30 /**Vitesse d'attaque' du personnage en pixel par tick*/

#define JPCD 4 /**Délai avant de pouvoir de nouveau sauter */

#define NBPXSAUT 5*TAILLEBLOC /**Taille d'un saut en pixel */

#define ZONEMORTE 5000 /**Zone morte de la manette */

#define NIVEAUTXT "salle_yourte.txt" /**Fichier du niveau de départ*/

#define FRAMEDELAY 17 /**Correspond à du 59fps */

#define TAILLEKONAMI 11 /**NB d'inputs du Konami code */
#define KONAMICODE "uuddlrlrbas" /**Inputs du Konami code */

#define NBBLOCSTILESET 60 /**Nombre de blocs du tileset de blocs*/
#define NBLOCERREUR 49 /**Indice du bloc d'erreur de la tileset*/

#define COEURDROPRATE 10 /**Pourcentage de probabilité de drop d'un coeur*/

#define PVMAX 4 /**Points de vie maximum du joueur*/
#define TEMPINV 60 /**Durée d'invincibilité du joueur après avoir été touché*/
#define FREQCLIGN 2 /**Fréquence de clignotement lors de l'invincibilité*/
#define INVUDELAY 6 /**Delai de l'invulnérabilité*/

#define FONTSIZE 30 /**Taille de la police lors de la création du texte*/

#define NBETATSBOUTONS 4 /**Nombre d'états différents des boutons de menu*/
#define NBETATS 5 /**Nombre d'états différents des entités/joueurs */

#define ANIMDELAYMENU 20 /**Delai de changement d'états pour les boutons*/

#define PULSARWAV "audio/Pulsar.wav"
#define BEGINWAV "audio/Begin.wav"
#define MYSTERIOUSWAV "audio/Mysterious.wav"
#define FUTURISTICWAV "audio/Futuristic.wav"
#define ENDLESSPAINWAV "audio/EndlessPain.wav"
#define LONGAWAYWAV "audio/LongAway.wav"
#define DEFAULTVOLUME 0.2 /**Volume par défaut de la musique*/
#define NBCHANNELS 20 /**Nombre de sons jouables en même temps */


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
 * \enum idSounds_e
 * \brief Id des sons produits par le jeu
*/
typedef enum idSounds_e{
  SOUND_TOUCHE, /**Son quand le personnage est touché */
  SOUND_ARC, /**Son quand le personnage tire à l'arc*/
  SOUND_SERPENT, /**Son quand le serpent rose crache*/
  SOUND_SINGE, /**Son quand le singe crie*/
  SOUND_VIFPLUME, /**Son quand le vifplume crie*/
  SOUND_MURGLACE, /**Son quand le mur de glace fond*/
  SOUND_DOOR, /**Son quand une porte s'ouvre*/
  SOUND_ITEM, /**Son quand on ramasse un item*/
  SOUND_MENU /**Son quand on valide un bouton dans le menu*/
} idSounds_t;



/*typedef struct audiodata_s{
  SDL_AudioDeviceID deviceId;
  SDL_AudioSpec wav_spec;
  Uint32 wav_length;
  Uint8 *wav_buffer;
  Uint32 audioPos;
  Uint32 audioLen;
  Uint8 *audioBufferPos;
  int volume;
}audiodata_t;*/

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
 * \brief Nom des différents états possibles du personnage et des entités (utiles pour l'animation de sprites et la gestion des déplacements et des hitbox)
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
 * \brief Structure représentant les salles et leur contenu
*/
typedef struct salle_s{
    char * nomFichier; /**< Nom du fichier qui a généré la salle (utile pour les sauvegardes) */
    int hauteur; /**< Hauteur de la salle en blocs */
    int largeur; /**< Longueur de la salle en blocs */
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
 * \brief Structure représentant le personnage et ses caractéristques, contient également une variable de gestion des effets sonores du jeu
*/
typedef struct personnage_s{
    int pv; /**< PV (points de vie) actuel du personnage */
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
    position_t delta; /**< Position en pixel à l'intérieur de la case de matrice (entre 0 et TAILLEBLOC-1) */
    position_t apparition; /**< Position d'apparition (utilisé pour les sauvegardes) */
    SDL_Texture * sprites; /**Pointeur vers la texture qui contient les sprites du personnage */
    SDL_Texture * spritesR; /**Pointeur vers la texture qui contient les sprites du renard */
    SDL_Rect spriteActuel; /**< Indice du sprite actuel en x et y dans la texture */
    SDL_Rect * hitbox; /**< Taille de la hitbox du personnage en pixel */
    int hitboxActuelle; /** Indice de la hitbox actuellement utilisée (0 pour le personnage et 1 pour le renard) */
    int nbHitbox; /**Nombre de hitbox du personnage */
    etat_t etat; /**< Etat du personnage (idle/running/jumping/attacking/falling) */
    boolean_t newEtat; /**< Booléen qui signifie qu'un changement d'état vient de s'effectuer */
    boolean_t newItem; /**< Booléen qui signifie que l'on vient de récolter un item */
    int evoSprite; /**< Entier qui décrémente, changement de sprite quand vaut 0 */
    int * nbAnim; /**< Tableau qui contient le nombre de sprites d'animation pour chaque action du personage */
    int * vitAnim; /**< Tableau qui contient le delai de changement d'animation pour chaque action du personage */
    char forme; /**< Forme du personnage h = humain, r = renard */
    int inventaire[TAILLE_INVENTAIRE]; /**<Tableau qui contient les informations sur l'inventaire actuel du personnage */
    SDL_Texture * inventaireTileset; /**Tileset de l'inventaire*/
    char* nomObj[TAILLE_INVENTAIRE]; /**<Tableau qui contient les noms des objets de l'inventaire */
    unsigned int sounds; /**< Nombre binaire correspondant aux effets sonores à jouer pour la frame en cours */
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
    SDL_Rect * hitbox; /**< Tableau de hitboxs de l'entite (décallage en x, en y, largeur de la hitbox, hauteur de la hitbox) */
    int nbHitbox; /**Nombre de rectangles d'hitbox différents pour l'entité */
    taille_t tailleSprite; /**< Taille des sprites de monstre en pixel */
    boolean_t passeEntites; /**< Indique si le monstre peut passer à travers les entités (autres monstres/joueur) */
    boolean_t passeBlocs; /**< Indique si le monstre peut passer à travers les blocs */
    int radius; /**Nombre correspondant au champ de vision de l'entité */
    int vitesseAnim; /**< Vitesse de l'animation des sprites (plus ce nombre est grand, plus la vitesse est lente (sert à evoSprite comme valeur avant décrémentation)) */
    void (*comportement)(monstre_t* entite, personnage_t* perso, salle_t* salle); /** Fonction de comportement (pointeur sur fonction) */
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


/**
 * \enum boutonetat_e
 * \brief Id des états de boutons
*/
typedef enum boutonetat_e{
  RELAXED, /**Etat du bouton lorsqu'il n'est pas touché*/
  HIGHLIGHTED, /**Etat du bouton lorsqu'il est survolé*/
  PRESSED, /**Etat du bouton lorsqu'il est pressé*/
  UNAVAILABLE /**Etat du bouton lorsqu'il n'est pas disponible*/
} boutonetat_t;


/**
 * \struct menu_bouton_s
 * \brief Structure représentant un bouton d'un menu
*/
typedef struct menu_bouton_s{
  int id; /** Id du bouton */
  char * etiquette; /** Etiquette du bouton */
  boutonetat_t etat; /** Etat du bouton */
  SDL_Texture ** texture; /** Texture du bouton */
  SDL_Rect emplacement; /** Emplacement du bouton */
} menu_bouton_t;

/**
 * \struct menu_texte_s
 * \brief Structure représentant un texte dans un menu
*/
typedef struct menu_texte_s{
  int id; /** Id du texte */
  char * etiquette; /** Etiquette du texte */
  SDL_Texture * texture; /** Texture du texte */
  SDL_Rect emplacement; /** Emplacement du texte */
} menu_texte_t;

/**
 * \struct menu_s
 * \brief Structure représentant un menu
*/
typedef struct menu_s{
  char * etiquette; /** Etiquette du menu */
  menu_bouton_t * tabBoutons; /** Tableau de boutons */
  int nbBoutons; /** Nombre de boutons dans le menu */
  int idBoutonChoisi; /** Id du bouton actuellement choisi */
  int idBoutonValide; /** id du bouton actuellement validé */
  SDL_Texture * fond; /** Texture du fond du menu */
  int nbSprites; /** Nombre de sprites du fond du menu */
  SDL_Rect spriteActuel; /** Sprite actuellement affiché au fond du menu */
  int animDelay; /** Delai entre le changement d'animation du menu */
  int etatanim; /** Nombre qui va de 0 à animDelay */
  menu_texte_t * tabTextes; /** Tableau des textes du menu */
  int nbTextes; /** Nombre de textes du menu*/
} menu_t;

#endif
