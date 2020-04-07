#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../headers/structs.h"
#include "../headers/source.h"

//Mis en commentaire des fonctions car les structures ont changé

void mainInit(personnage_t** perso, liste_t** entites){
    /**perso = malloc(sizeof(personnage_t));
    *entites = creerListe("monstre");

    //init du perso
    (*perso)->vit_dep = 1;
    (*perso)->vit_att = 0;
    (*perso)->nb_sprites = 17;
    //l'inventaire
    for(int i = 0; i < TAILLE_INVENTAIRE; i++)
        (*perso)->inventaire[i] = 0;
    //les objets de l'inventaire
    (*perso)->nomObj[0] = "champignon";
    (*perso)->nomObj[1] = "cle bleue";
    (*perso)->nomObj[2] = "cle rouille";
    (*perso)->nomObj[3] = "cle rouge";
    (*perso)->nomObj[4] = "cle verte";
    (*perso)->nomObj[5] = "double saut";
    (*perso)->nomObj[6] = "fleche de feu";
    (*perso)->nomObj[7] = "renard";
    */
    //load sprites

}

void initSdlSalle(salle_t* salle){
    /*SDL_Surface*** sprites = malloc(sizeof(SDL_Surface**) * salle->hauteur);
    char* tmp = NULL;

    for(int i = 0; i < salle->hauteur; i++){
        sprites[i] = malloc(sizeof(SDL_Surface*) * salle->largeur);
        for(int j = 0; j < salle->largeur; j++){
            tmp = chercherSprite(salle->mat[i][j],"./sprites/");
            if(tmp){
                sprites[i][j] = IMG_Load(tmp);
                free(tmp);
            }else
                sprites[i][j] = NULL;
        }
    }
    salle->sprites = sprites;
    */
}

void freeSdlSalle(salle_t* salle){
    /*for(int i = 0; i < salle->hauteur; i++){
        for(int j = 0; j < salle->largeur; j++)
            if(salle->sprites[i][j])
                SDL_FreeSurface(salle->sprites[i][j]);
        free(salle->sprites[i]);
    }

    free(salle->sprites);
    salle->sprites = NULL;
    */
}

void initCreerPartie(personnage_t* perso, salle_t** salle){
    /*perso->pv = perso->pv_max = 4;
    perso->delta.delta_x.numerateur = perso->delta.delta_y.numerateur = 0;
    perso->delta.delta_x.denominateur = perso->delta.delta_y.denominateur = 8; //TAILLEBLOC;
    perso->spriteActuel = SU;
    perso->forme = 'H';
    perso->pos = (position_t){8,13};

    lireSalle("salle_debut.txt",salle);
    initSdlSalle(*salle);
    */
}

void quitterPartie(salle_t* salle){
    freeSdlSalle(salle);
}

void initChargerSauv(){
    //a faire plus tard
}
