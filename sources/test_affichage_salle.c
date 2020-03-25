#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../headers/structs.h"
#include "../headers/source.h"

int main(int argc, char* argv[]){
    salle_t* salle = NULL;
    SDL_Surface*** sprites = NULL;
    char* tmp;

    lireSalle("salle_test1.txt",&salle);

    sprites = malloc(sizeof(SDL_Surface**) * salle->hauteur);

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

    //code sdl
    /*
        ici tu peux utiliser sprites qui est une matrice de pointeur sur SDL_Surface
        chaque sprites[i][j] correspond au bloc à afficher à la position i*8,j*8
    */

    for(int i = 0; i < salle->hauteur; i++){
        for(int j = 0; j < salle->largeur; j++)
            if(sprites[i][j])
                SDL_FreeSurface(sprites[i][j]);
        free(sprites[i]);
    }

    free(sprites);

    for(int i = 0; i < salle->hauteur; i++)
        free(salle->mat[i]);
    free(salle->mat);
    supListe(&salle->listePorte,supPorte);

    free(salle->nomFichier);
    free(salle);

    return 0;
}