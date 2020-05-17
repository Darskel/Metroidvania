//opendir
#include <dirent.h>
#include <errno.h>
//stat
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/source.h"
#include "../headers/liste.h"
#include "../headers/comportement.h"

/**
 * \file source.c
 * \brief Fonctions de lecture d'une salle
 * \author Thomas DIDIER & Marie-Nina MUNAR L2 Info Le Mans
 * \version 3.0
 * \date 17/05/2020
*/

/**
 * \brief Sauvegarde l'état de la partie
 *
 * @param numSauv numéro de la sauvegarde (entre 1 et 3)
 * @param perso pointeur vers le personnage (structure personnage)
 * @param salle nom de la dernière salle visité par le personnage principal
 *
 * @return un code erreur en fonction de l'erreur: -1 = Mauvais numéro de sauvegarde, -2 = impossible d'ouvrir ou créer le fichier; ou le numéro de sauvegarde dans laquelle la sauvegarde a été effectuée (avec succès)
*/
int sauvegarder(int numSauv, personnage_t* perso, char* salle){
    if(numSauv < 1 || numSauv > 3)
        return -1; // Mauvais numéro de sauvegarde

    char cmd[20] = "mkdir ";

    //avec opendir
    DIR* dir = opendir(DIR_SAUV);
    if(dir)
        closedir(dir);
    else
        if(ENOENT == errno){
            //CREATE_DIR(DIR_SAUV);
            strcat(cmd, DIR_SAUV);
            system(cmd);
        }

    char* nomFichier = malloc(sizeof(char) * (6 + strlen(DIR_SAUV) + 3));
    strcpy(nomFichier, "./");
    strcat(nomFichier, DIR_SAUV);
    strcat(nomFichier, "/");
    //itoa(numSauv, cmd, 10);
    cmd[0] = numSauv + '0';
    cmd[1] = '\0';
    strcat(nomFichier, cmd);
    strcat(nomFichier, ".txt");

    FILE * file = fopen(nomFichier,"w");
    free(nomFichier);
    if(!file)
        return -2; // Le fichier n'a pas pû être ouvert ou créé

    //printf("Path salle sauvegardée : %s\n", salle);
    //Ecrit les hp, la salle et la position de réaparition
    fprintf(file, "Health Point: %d\nNom de la salle: %s\nPosition: %d %d\nInventaire:\n", perso->pv, salle, perso->apparition.x, perso->apparition.y);

    for(int i = 0; i < TAILLE_INVENTAIRE; i++)
        fprintf(file, "\t%s: %d\n", perso->nomObj[i], perso->inventaire[i] ? 1 : 0);

    fclose(file);
    return numSauv;
}

/**
 * \brief Charge l'état de la partie
 * \details Les paramètres numSauv et nomObjs doivent être définis avant, les autres paramètres seront remplis par cette fonction
 *
 * @param numSauv numéro de la sauvegarde (entre 1 et 3)
 * @param perso personnage (structure personnage_t) à remplir
 * @param salle nom de la salle à charger
 *
 * @return un code erreur en fonction de l'erreur: -1 = Mauvais numéro de sauvegarde, -2 = impossible d'ouvrir ou créer le fichier; le numéro de sauvegarde dans laquelle la sauvegarde a été effectuée (avec succès), ou 0 si le fichier de sauvegarde est vide
*/
int chargerSauvegarde(int numSauv, personnage_t* perso, char* salle){
    if(numSauv < 1 || numSauv > 3)
        return -1; // Mauvais numéro de sauvegarde

    //avec opendir
    DIR* dir = opendir(DIR_SAUV);
    if(dir)
        closedir(dir);
    else
        if(ENOENT == errno)
            return 0; // Le dossier n'existe pas (et donc la sauvegarde n'existe pas)

    char tmp[20];
    char* nomFichier = malloc(sizeof(char) * (6 + strlen(DIR_SAUV) + 3));
    strcpy(nomFichier, "./");
    strcat(nomFichier, DIR_SAUV);
    strcat(nomFichier, "/");
    //itoa(numSauv, tmp, 10);
    tmp[0] = numSauv + '0';
    tmp[1] = '\0';
    strcat(nomFichier, tmp);
    strcat(nomFichier, ".txt");

    FILE * file = fopen(nomFichier,"r");
    free(nomFichier);
    if(!file)
        return -2; // Le fichier n'a pas pû être ouvert

    fscanf(file, "%c", tmp);

    if(feof(file)){
        fclose(file);
        return 0; //Le fichier de sauvegarde est vide
    }

    fscanf(file, "ealth Point:%d\nNom de la salle: %s\nPosition: %d %d\nInventaire:\n", &(perso->pv), salle, &(perso->apparition.x), &(perso->apparition.y));
    perso->pos.x=perso->apparition.x;
    perso->pos.y=perso->apparition.y;
    //printf("Path salle chargée : %s\n", salle);

    for(int i = 0; i < TAILLE_INVENTAIRE; i++){
        fscanf(file,"%[^:]: ", tmp);
        if(!strcmp(tmp,perso->nomObj[i]))
            fscanf(file,"%d\n", perso->inventaire + i);
        else{
            int j;
            for(j = 0; j < TAILLE_INVENTAIRE && strcmp(tmp,perso->nomObj[j]); j++);
            if(j != TAILLE_INVENTAIRE){
                fscanf(file,"%d\n", perso->inventaire + j);
            }
        }
    }

    fclose(file);
    return numSauv;
}

/**
 * \brief Créé le tableau de types d'entités
 * \details chaque entité est créée en dur dans le code
 *
*/
void creerTypeEntite(){

    //clé bleue
    int* tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;
    SDL_Rect * hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,11,10};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,0,0};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-CLEBLEUE - 1] = (type_monstre_t){
        1, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "cle bleue", //nom de l'entité
        "sprites/entite/cles/cle_bleue.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {10,11}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs
        0, //Champ de vision
        0, //N'attend pas avant de changer d'animation
        compRecuperable //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //clé rouge
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,11,10};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,0,0};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-CLEROUGE - 1] = (type_monstre_t){
        1, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "cle rouge", //nom de l'entité
        "sprites/entite/cles/cle_rouge.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {10,11}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        0, //N'attend pas avant de changer d'animation
        compRecuperable //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //clé rouillée
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,11,10};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,0,0};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-CLEROUILLE - 1] = (type_monstre_t){
        1, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "cle rouillee", //nom de l'entité
        "sprites/entite/cles/cle_rouille.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {10,11}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        0, //N'attend pas avant de changer d'animation
        compRecuperable //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //clé verte
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,11,10};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,0,0};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-CLEVERTE - 1] = (type_monstre_t){
        1, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "cle verte", //nom de l'entité
        "sprites/entite/cles/cle_verte.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {10,11}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        0, //N'attend pas avant de changer d'animation
        compRecuperable //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //dischoshroom
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 8;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,16,16};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,0,0};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-DISCOSHROOM - 1] = (type_monstre_t){
        1, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "discoshroom", //nom de l'entité
        "sprites/entite/discoshroom/tileset.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {16,16}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        18, //Attend ce nombre de frame avant de changer d'animation (vitesse d'animation)
        compRecuperable //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //double saut
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 11;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,25,25};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,0,0};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-DOUBLESAUT - 1] = (type_monstre_t){
        1, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "double saut", //nom de l'entité
        "sprites/entite/double-saut/tileset.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {25,25}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        15, //Attend ce nombre de frame avant de changer d'animation (vitesse d'animation)
        compRecuperable //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //huile
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,8,8};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,0,0};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-FLACONHUILE - 1] = (type_monstre_t){
        1, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "huile", //nom de l'entité
        "sprites/entite/flacon_huile/flacon.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {8,8}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        0, //N'attend pas avant de changer d'animation
        compRecuperable //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //mur de glace
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 10;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,16,32};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,0,0};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-MURGLACE - 1] = (type_monstre_t){
        2, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "mur glace", //nom de l'entité
        "sprites/entite/mur_glace/tileset.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {32,16}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        0, //N'attend pas avant de changer d'animation
        compMurGlace //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //porte bleue
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 1;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,4,36};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,13,36};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-PORTEB - 1] = (type_monstre_t){
        1, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "porte bleue", //nom de l'entité
        "sprites/entite/portes/porte_bleu.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {36,13}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        0, //N'attend pas avant de changer d'animation
        compPortes //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //porte rouge
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 1;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,4,36};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,13,36};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-PORTERG - 1] = (type_monstre_t){
        1, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "porte rouge", //nom de l'entité
        "sprites/entite/portes/porte_rouge.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {36,13}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        0, //N'attend pas avant de changer d'animation
        compPortes //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //porte rouillée
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 1;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,4,36};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,13,36};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-PORTERL - 1] = (type_monstre_t){
        1, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "porte rouillee", //nom de l'entité
        "sprites/entite/portes/porte_rouillee.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {36,13}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        0, //N'attend pas avant de changer d'animation
        compPortes //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //porte verte
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 1;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,4,36};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,13,36};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-PORTEV - 1] = (type_monstre_t){
        1, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "porte verte", //nom de l'entité
        "sprites/entite/portes/porte_verte.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {36,13}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        0, //N'attend pas avant de changer d'animation
        compPortes //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //renard
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,24,13};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,0,0};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-RENARD - 1] = (type_monstre_t){
        1, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "renard", //nom de l'entité
        "sprites/entite/renard/tileset.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {13,24}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        0, //N'attend pas avant de changer d'animation
        compRecuperable //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //roi vifplume
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 2;
    tmp[1] = 1;
    tmp[2] = 0;
    tmp[3] = 2;
    hittmp = malloc(sizeof(SDL_Rect) * 2 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,22,57};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,22,57};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,22,57};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,22,57};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};
    hittmp[IDLE+NBETATS]=(SDL_Rect){16,4,6,49};
    hittmp[RUNNING+NBETATS]=(SDL_Rect){20,34,30,11};
    hittmp[JUMPING+NBETATS]=(SDL_Rect){0,0,22,57};
    hittmp[ATTACKING+NBETATS]=(SDL_Rect){21,8,16,44};
    hittmp[FALLING+NBETATS]=(SDL_Rect){0,0,0,0};

    typesMonstre[-ROIVP - 1] = (type_monstre_t){
        7, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "roi vifplume", //nom de l'entité
        "sprites/entite/roi_vifplume/tileset.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        2, //Nombre de rectangles d'hitbox différents pour l'entité
        {57,50}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        25*TAILLEBLOC, //Champ de vision
        25,//Attend ce nombre de frame avant de changer d'animation (vitesse d'animation)
        compRoiVifplume //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //Serpent bleu
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 2;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 2 *NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){35,1,14,29};
    hittmp[RUNNING+0]=(SDL_Rect){35,1,14,29};
    hittmp[JUMPING+0]=(SDL_Rect){35,1,14,29};
    hittmp[ATTACKING+0]=(SDL_Rect){35,1,14,29};
    hittmp[FALLING+0]=(SDL_Rect){35,1,14,29};
    hittmp[IDLE+NBETATS]=(SDL_Rect){0,23,49,7};
    hittmp[RUNNING+NBETATS]=(SDL_Rect){0,23,49,7};
    hittmp[JUMPING+NBETATS]=(SDL_Rect){0,23,49,7};
    hittmp[ATTACKING+NBETATS]=(SDL_Rect){0,23,49,7};
    hittmp[FALLING+NBETATS]=(SDL_Rect){0,23,49,7};

    typesMonstre[-SERPENTBLEU - 1] = (type_monstre_t){
        2, //pv de base
        VITDEPPERS > 1 ? VITDEPPERS/2 : 1, //vit de deplacement
        0, //vitesse d'attaque
        "serpent bleu",//nom de l'entité
        "sprites/entite/serpent_bleu/tileset.png",//chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //{0,2,0,0} //Tableau de nombre d'animations par etat
        1, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        2, //Nombre de rectangles d'hitbox différents pour l'entité
        {30,51}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        25,//Attend ce nombre de frame avant de changer d'animation (vitesse d'animation)
        compSerpent //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //Serpent rose
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 0;
    tmp[1] = 2;
    tmp[2] = 0;
    tmp[3] = 1;
    hittmp = malloc(sizeof(SDL_Rect) * 2 *NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){35,1,14,29};
    hittmp[RUNNING+0]=(SDL_Rect){35,1,14,29};
    hittmp[JUMPING+0]=(SDL_Rect){35,1,14,29};
    hittmp[ATTACKING+0]=(SDL_Rect){35,1,14,29};
    hittmp[FALLING+0]=(SDL_Rect){35,1,14,29};
    hittmp[IDLE+NBETATS]=(SDL_Rect){0,23,49,7};
    hittmp[RUNNING+NBETATS]=(SDL_Rect){0,23,49,7};
    hittmp[JUMPING+NBETATS]=(SDL_Rect){0,23,49,7};
    hittmp[ATTACKING+NBETATS]=(SDL_Rect){0,23,49,7};
    hittmp[FALLING+NBETATS]=(SDL_Rect){0,23,49,7};

    typesMonstre[-SERPENTROSE - 1] = (type_monstre_t){
        2, //pv de base
        VITDEPPERS > 1 ? VITDEPPERS/2 : 1, //vit de deplacement
        100, //vitesse d'attaque
        "serpent rose",//nom de l'entité
        "sprites/entite/serpent_rose/tileset.png",//chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //{0,2,0,0} //Tableau de nombre d'animations par etat
        0, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        2, //Nombre de rectangles d'hitbox différents pour l'entité
        {30,51}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        25*TAILLEBLOC, //Champ de vision
        25, //Attend ce nombre de frame avant de changer d'animation (vitesse d'animation)
        compSerpentRose //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //Serpent vert
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 2;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 2 *NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){35,1,14,29};
    hittmp[RUNNING+0]=(SDL_Rect){35,1,14,29};
    hittmp[JUMPING+0]=(SDL_Rect){35,1,14,29};
    hittmp[ATTACKING+0]=(SDL_Rect){35,1,14,29};
    hittmp[FALLING+0]=(SDL_Rect){35,1,14,29};
    hittmp[IDLE+NBETATS]=(SDL_Rect){0,23,49,7};
    hittmp[RUNNING+NBETATS]=(SDL_Rect){0,23,49,7};
    hittmp[JUMPING+NBETATS]=(SDL_Rect){0,23,49,7};
    hittmp[ATTACKING+NBETATS]=(SDL_Rect){0,23,49,7};
    hittmp[FALLING+NBETATS]=(SDL_Rect){0,23,49,7};

    typesMonstre[-SERPENTVERT - 1] = (type_monstre_t){
        3, //pv de base
        VITDEPPERS > 1 ? VITDEPPERS/2 : 1, //vit de deplacement
        0, //vitesse d'attaque
        "serpent bleu",//nom de l'entité
        "sprites/entite/serpent_vert/tileset.png",//chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //{0,2,0,0} //Tableau de nombre d'animations par etat
        1, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        2, //Nombre de rectangles d'hitbox différents pour l'entité
        {30,51}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        25, //Attend ce nombre de frame avant de changer d'animation (vitesse d'animation)
        compSerpent //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //singe de grotte
    tmp = malloc(sizeof(int) * 5);
    tmp[0] = 1;
    tmp[1] = 0;
    tmp[2] = 2;
    tmp[3] = 1;
    tmp[4] = 2;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,19,26};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,19,26};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,21,31};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,25,35};
    hittmp[FALLING+0]=(SDL_Rect){0,0,25,31};


    typesMonstre[-SINGEGROTTE - 1] = (type_monstre_t){
        4, //pv de base
        1, //vit de deplacement
        0, //vitesse d'attaque
        "singe grotte",//nom de l'entité
        "sprites/entite/singe_grotte/tileset.png",//chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //{0,2,0,0} //Tableau de nombre d'animations par etat
        1, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur) {35,25}, {32,22}, {31,25}, {31,21} quand il saute !
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {35,25}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        25*TAILLEBLOC, //Champ de vision
        10, //Attend ce nombre de frame avant de changer d'animation (vitesse d'animation)
        compSingeGrotte //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //vers géant
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 8;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,1,1};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,0,0};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-VERSGEANT - 1] = (type_monstre_t){
        2, //pv de base
        0, //vit de deplacement
        0, //vitesse d'attaque
        "vers geant",//nom de l'entité
        "sprites/entite/vers_geant/tileset.png",//chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //{0,2,0,0} //Tableau de nombre d'animations par etat
        1, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur) {2,5}, {8,7}, {13,8}, {17,10}, {21,10}, {25,10}, {29,10} quand il attaque ! +4,+0 à partir de [4]
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {33,20}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        25, //Attend ce nombre de frame avant de changer d'animation (vitesse d'animation)
        compVersGeant //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //vifplume
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 8;
    tmp[2] = 0;
    tmp[3] = 1;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,21,30};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,32,26};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,21,30};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,32,26};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-VIFPLUME - 1] = (type_monstre_t){
        2, //pv de base
        2, //vit de deplacement
        50, //vitesse d'attaque
        "vifplume", //nom de l'entité
        "sprites/entite/vifplume/tileset.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //{0,2,0,0} //Tableau de nombre d'animations par etat
        1, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur) {26,32} quand il attaque !
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {31,37}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        25*TAILLEBLOC, //Champ de vision
        8, //Attend ce nombre de frame avant de changer d'animation (vitesse d'animation)
        compVifplume //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //coeur
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,8,8};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,0,0};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-COEUR - 1] = (type_monstre_t){
        1, //pv de base
        1, //vit de deplacement
        0, //vitesse d'attaque
        "coeur", //nom de l'entité
        "sprites/entite/coeur/coeur.png", //chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        1, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {8,8}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        0, //N'attend pas avant de changer d'animation
        compCoeur //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //fleche
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 2;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,19,3};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,19,3};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-FLECHE - 1] = (type_monstre_t){
        1, //pv de base
        3, //vit de deplacement
        0, //vitesse d'attaque
        "fleche",//nom de l'entité
        "sprites/entite/fleche/tileset.png",//chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        1, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {9,19}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        5, //Attend ce nombre de frame avant de changer d'animation (vitesse d'animation)
        compFleches //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //fleche de feu
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 2;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,19,5};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,19,5};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-FLECHEFEU - 1] = (type_monstre_t){
        1, //pv de base
        3, //vit de deplacement
        0, //vitesse d'attaque
        "fleche feu",//nom de l'entité
        "sprites/entite/fleche_feu/tileset.png",//chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        2, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {8,19}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        5, //Attend ce nombre de frame avant de changer d'animation (vitesse d'animation)
        compFleches //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };

    //venin
    tmp = malloc(sizeof(int) * 4);
    tmp[0] = 1;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;
    hittmp = malloc(sizeof(SDL_Rect) * 1 * NBETATS);
    hittmp[IDLE+0]=(SDL_Rect){0,0,7,6};
    hittmp[RUNNING+0]=(SDL_Rect){0,0,0,0};
    hittmp[ATTACKING+0]=(SDL_Rect){0,0,0,0};
    hittmp[JUMPING+0]=(SDL_Rect){0,0,0,0};
    hittmp[FALLING+0]=(SDL_Rect){0,0,0,0};

    typesMonstre[-VENIN - 1] = (type_monstre_t){
        1, //pv de base
        3, //vit de deplacement
        0, //vitesse d'attaque
        "venin",//nom de l'entité
        "sprites/entite/venin/crachat.png",//chemin vers les sprites
        NULL, //SDL_Texture* sprites non initialisé !
        tmp, //Tableau de nombre d'animations par etat
        1, //nombre de dégats qu'il inflige
        hittmp, //hitbox de l'entité (x,y,largeur,hauteur)
        1, //Nombre de rectangles d'hitbox différents pour l'entité
        {6,7}, //taille sprites
        FALSE, //Passe à travers les entités
        FALSE, //Passe à travers les blocs*
        0, //Champ de vision
        0, //N'attend pas avant de changer d'animation
        compVenin //comportement à rajouter avec un la fonction (pointeur sur la fonction)
    };
}

/**
 * \brief Permet d'obtenir le tableau des types d'entités
 *
 * @return le tableau de types d'entités
*/
type_monstre_t* obtenirTypesEntite(){
    return typesMonstre;
}

/**
 * \brief Créé une entité et l'ajoute à la liste des entités dans la salle
 *
 * @param id id correspondant au type de l'entite
 * @param s pointeur sur la structure salle où trouver la liste
 * @param pos la position de l'entite (en cases)
 * @param p personnage (utile pour contrôler l'inventaire)
*/
static void creerEntite(idEnt_t id, salle_t* s, position_t pos, personnage_t* p){
    monstre_t* e = malloc(sizeof(monstre_t));
    id *= -1;

    e->type = &typesMonstre[id-1];
    e->pv = e->type->pv_base;
    e->ut = 0;
    e->cdAtt = 0;


    if(e->type->comportement == compRecuperable){
        for(int i = 0; i < TAILLE_INVENTAIRE; i++){
            if(!strcmp(e->type->nom,p->nomObj[i])){
                if(p && p->inventaire[i]){
                    free(e);
                    return;
                }
            }
        }
    }

    e->delta.x =0;
    e->delta.y = (TAILLEBLOC - e->type->tailleSprite.hauteur%TAILLEBLOC)%TAILLEBLOC;
    e->pos = pos;
    e->direction = LEFT;
    e->etat = RUNNING;
    e->newEtat = TRUE;
    e->evoSprite = 0;

    e->spriteActuel.h = e->type->tailleSprite.hauteur;
    e->spriteActuel.w = e->type->tailleSprite.largeur;
    e->spriteActuel.x = 0;
    e->spriteActuel.y = e->etat * e->spriteActuel.h;

    monstre_t tmp;

    enTete(s->listeEntite);
    do{
        valeurElm(s->listeEntite,&tmp);
        suivant(s->listeEntite);
    }while(tmp.type->comportement != compPortes && !horsListe(s->listeEntite));

    if(e->type->comportement == compPortes)
        if(!horsListe(s->listeEntite))
          ajoutDroit(s->listeEntite, e);
        else{
          enQueue(s->listeEntite);
          ajoutGauche(s->listeEntite, e);
        }
    else{
        if(horsListe(s->listeEntite)){
          enQueue(s->listeEntite);
          ajoutGauche(s->listeEntite, e);
        }
        else{
          ajoutDroit(s->listeEntite, e);
        }
    }
}

/**
 * \brief Nettoie et supprime la structure salle donnée
 * \details libère toute la mémoire utiliser et prépare le pointeur à être de nouveau utilisé
 *
 * @param salle pointeur sur le pointeur de la structure salle à nettoyer
 * @return 0 si tout s'est bien déroulé
*/
int nettoyerSalle(salle_t** salle){
    for(int i = 0; i < (*salle)->hauteur; i++)
        free((*salle)->mat[i]);

    supListe(&((*salle)->listePorte),supPorte);
    supListe(&((*salle)->listeEntite),supMonstre);
    free((*salle)->mat);
    free((*salle)->nomFichier);
    free(*salle);
    *salle = NULL;

    return 0;
}

/**
 * \brief Modifie la structure salle pour la nouvelle salle
 *
 * @param nomFichier chaîne de caractères correspondant au nom du fichier à lire
 * @param salle pointeur du pointeur de la structure salle à remplir avec la lecture
 * @param perso le personnage à inclure dans la salle
 *
 * @return 0 si tout s'est bien déroulé
*/
int lireSalle(char* nomFichier, salle_t** salle, personnage_t* perso){
    FILE * monDoc = NULL;
    char mot[50];
    int lon, larg, val;
    int nbanim, vitanim;
    int cx1, cx2, cy1, cy2;

    if(*salle!=NULL)
        nettoyerSalle(salle);

    char tmp[100] = DIR_SALLE;
    strcat(tmp,nomFichier);

    //creation des variables dans le tas
    monDoc = fopen(tmp, "r");
    if(!monDoc)
        return 1;

    *salle = malloc(sizeof(salle_t));
    (*salle)->nomFichier = malloc(sizeof(char) * (strlen(nomFichier)+1));
    strcpy((*salle)->nomFichier, nomFichier);

    //Nb anims
    fscanf(monDoc, "%d %d", &nbanim, &vitanim);
    (*salle)->animDelay=vitanim;
    (*salle)->etatanim=(*salle)->animDelay;
    (*salle)->nbsprites=nbanim;

    //taille matrice
    fscanf(monDoc, "%d %d", &lon, &larg);
    //printf ("%d, %d \n", lon, larg);

    (*salle)->largeur = lon;
    (*salle)->hauteur = larg;

    //Création matrice
    (*salle)->mat = malloc(sizeof(int*) * larg);
    for(int i = 0; i < larg; i++)
        (*salle)->mat[i] = malloc(sizeof(int) * lon);

    (*salle)->listeEntite = creerListe("monstre");

    //Remplissage matrice
    for(int i = 0; i < lon*larg; i++){
        fscanf(monDoc, "%d", &val);
        //gestion des entités !
        if(val < 0 && val >= -NBTYPEMONSTRE){
            creerEntite(val,*salle, (position_t){i%lon,i/lon}, perso);
            (*salle)->mat[i/lon][i%lon] = 0;
        }else
            (*salle)->mat[i/lon][i%lon] = val > 0 ? val : 0;
        //printf("\n %d %d \n", i/lon,i%lon);
    }

    (*salle)->listePorte = creerListe("porte");
    porte_t* p;

    //Création et remplissage des portes
    fscanf(monDoc, "%s %d %d %d %d", mot, &cx1, &cy1, &cx2, &cy2);
    while(!feof(monDoc)){
        p = malloc(sizeof(*p));
        p->pos.x = cx1;
        p->pos.y = cy1;
        p->pos_arrivee.x = cx2;
        p->pos_arrivee.y = cy2;
        p->salleSuivante = malloc(sizeof(char) * (strlen(mot) + 1));
        strcpy(p->salleSuivante, mot);
        //Gestion des sprites de portes potentiellement à modifier
        p->spriteActuel = -1;
        p->listeSprites = NULL;
        ajoutDroit((*salle)->listePorte, p);
        fscanf(monDoc, "%s %d %d %d %d", mot, &cx1, &cy1, &cx2, &cy2);
    }
    //fermeture fichier
    fclose(monDoc);
    return 0;
}
