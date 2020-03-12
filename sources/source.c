//opendir
#include <dirent.h>
#include <errno.h>
//stat
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/source.h"

/**
 * \file source.c
 * \brief Fonctions de lecture d'une salle
 * \author Thomas DIDIER L2 Info Le Mans
 * \version 1.0
 * \date 13/02/2020
*/

int findVal(int val){
    int r;
    while(val){
        r = val % BASE;
        val = val / BASE;
    }

    return r;
}

int filecmp(struct dirent f1, struct dirent f2){
    if(f1.d_type != f2.d_type){
        return f1.d_type ? -1 : 1;
    }

    return strcmp(f1.d_name,f2.d_name);
}

char* chercherSprite(int id, char* dirName){
    /* tout sur dirent ici: http://sdz.tdct.org/sdz/arcourir-les-dossiers-avec-dirent-h.html */
    if(!id)
        return NULL;

    char newName[50] = "";
    strcpy(newName, dirName);
    DIR* dir = opendir(dirName);
    struct dirent* file;
    if(!dir)
        return NULL; //dossier de sprites non trouvé ou inaccessible

    int val = findVal(id) - 1;
    int nbVal = 0;
    struct dirent * fileListe = malloc(sizeof(struct dirent*) * BASE);

    while((file = readdir(dir))){
        if(file->d_name[0] != '.'){
            printf("[%d]%d - fichier: %s\n",nbVal, file->d_type, file->d_name);
            fileListe[nbVal++] = *file;
        }
    }

    for(int i = 0; i < nbVal; i++)
        printf("[%d]%s\n",i,fileListe[i].d_name);

    qsort(fileListe, nbVal, sizeof(struct dirent), filecmp);

    for(int i = 0; i < nbVal; i++)
        printf("%s\n",fileListe[i].d_name);

    free(fileListe);
    closedir(dir);
    return NULL;
}

/**
 * \brief Sauvegarde l'état de la partie
 * 
 * @param numSauv numéro de la sauvegarde (entre 1 et 3)
 * @param hp nombre de point de vies du personnage principal
 * @param salle nom de la dernière salle visité par le personnage principal
 * @param dep position(position_t) d'arrivé dans la salle (position de départ après chargement)
 * @param inventaire tableau contenant l'état de l'inventaire du personnage
 * @param nomObjs tableau contenant le nom des objets de l'inventaire dans l'ordre
 * 
 * @return un code erreur en fonction de l'erreur: -1 = Mauvais numéro de sauvegarde, -2 = impossible d'ouvrir ou créer le fichier; ou le numéro de sauvegarde dans laquelle la sauvegarde a été effectuée (avec succès)
*/
int sauvegarder(int numSauv, int hp, char* salle, position_t* dep, int inventaire[], char* nomObjs[]){
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

    //Ecrit les hp, la salle et la position de réaparition
    fprintf(file, "Health Point: %d\nNom de la salle: %s\nPosition: %d %d\nInventaire:\n", hp, salle, dep->x, dep->y);
    
    for(int i = 0; i < TAILLE_INVENTAIRE; i++)
        fprintf(file, "\t%s: %d\n", nomObjs[i], inventaire[i] ? 1 : 0);

    fclose(file);
    return numSauv;
}

/**
 * \brief Sauvegarde l'état de la partie
 * \details Les paramètres numSauv et nomObjs doivent être définis avant, les autres paramètres seront remplis par cette fonction
 * 
 * @param numSauv numéro de la sauvegarde (entre 1 et 3)
 * @param salle nom de la salle à charger
 * @param perso personnage (structure personnage_t) à remplir
 * @param inventaire tableau contenant l'état de l'inventaire du personnage
 * @param nomObjs tableau contenant le nom des objets de l'inventaire dans l'ordre
 * 
 * @return un code erreur en fonction de l'erreur: -1 = Mauvais numéro de sauvegarde, -2 = impossible d'ouvrir ou créer le fichier; le numéro de sauvegarde dans laquelle la sauvegarde a été effectuée (avec succès), ou 0 si le fichier de sauvegarde est vide
*/

int chargerSauvegarde(int numSauv, char* salle, personnage_t* perso, int inventaire[], char* nomObjs[]){
    if(numSauv < 1 || numSauv > 3)
        return -1; // Mauvais numéro de sauvegarde

    //avec opendir
    DIR* dir = opendir(DIR_SAUV);
    if(dir)
        closedir(dir);
    else
        if(ENOENT == errno)
            return 0; // Le dossier n'existe pas (et donc aucune sauvegarde n'existe pas)

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

    fscanf(file, "ealth Point: %d\nNom de la salle: %s\nPosition: %d %d\nInventaire:\n", &(perso->pv), salle, &(perso->pos.x), &(perso->pos.y));
    
    for(int i = 0; i < TAILLE_INVENTAIRE; i++){
        fscanf(file,"%[^:]: ", tmp);
        printf("%s - %s\n", nomObjs[i], tmp);
        if(!strcmp(tmp,nomObjs[i]))
            fscanf(file,"%d\n", inventaire + i);
        else{
            while(strcmp(tmp,nomObjs[i])){//opti ici pour chercher l'indice correct (si indice non trouvé -> indice actuel inchangé)
                inventaire[i] = 0;
                i++;
            }
        }
    }

    fclose(file);
    return numSauv;
}

/**
 * \brief Nettoie et supprime la structure salle donnée
 * \details libère toute la mémoire utiliser et prépare le pointeur à être de nouveau utilisé
 * 
 * @param salle structure salle à traiter
*/
static int nettoyerSalle(salle_t** salle){
    for(int i = 0; i < (*salle)->hauteur; i++)
        free((*salle)->mat[i]);

    supListe(&((*salle)->listePorte));
    free((*salle)->mat);
    free((*salle)->nomFichier);
    free(*salle);
    *salle = NULL;

    return 0;
}

/**
 * \brief Modifie la structure salle pour la nouvelle salle
 * 
 * @param nomFichier nom du fichier de la salle à lire
 * @param salle structure salle du programme
*/
int lireSalle(char* nomFichier, salle_t** salle){
    FILE * monDoc = NULL;
    char mot[50];
    int lon, larg, val;
    int cx1, cx2, cy1, cy2;

    if(*salle)
        nettoyerSalle(salle);
    
    *salle = malloc(sizeof(salle_t));
    (*salle)->nomFichier = malloc(sizeof(char) * (strlen(nomFichier)+1));
    strcpy((*salle)->nomFichier, nomFichier);
    char tmp[20] = DIR_SALLE;
    strcat(tmp,nomFichier);

    //creation des variables dans le tas
    monDoc = fopen(tmp, "r");
    if(!monDoc)
        return 1;

    //taille matrice
    fscanf(monDoc, "%d %d", &lon, &larg);

    (*salle)->largeur = lon;
    (*salle)->hauteur = larg;

    //Création matrice
    (*salle)->mat = malloc(sizeof(int*) * lon);
    for(int i = 0; i < lon; i++)
        (*salle)->mat[i] = malloc(sizeof(int) * larg);

    //Remplissage matrice
    for(int i = 0; i < lon*larg; i++){
        fscanf(monDoc, "%d", &val);
        (*salle)->mat[i/larg][i%larg] = val;
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
        p->spritesActuel = -1;
        p->listeSprites = NULL;
        ajoutDroit((*salle)->listePorte, p);
        fscanf(monDoc, "%s %d %d %d %d", mot, &cx1, &cy1, &cx2, &cy2);
    }
    //fermeture fichier
    fclose(monDoc);
    return 0;
}