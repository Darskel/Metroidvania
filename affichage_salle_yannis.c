#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define LARGEUR_TILE 8 // hauteur et largeur des tiles.
#define HAUTEUR_TILE 8

#define NOMBRE_BLOCS_LARGEUR 15 // nombre a afficher en x et y
#define NOMBRE_BLOCS_HAUTEUR 16

char* table[] = {
    "000000000000000",
    "000000000000000",
    "000000000000000",
    "000000000000000",
    "000000000000000",
    "000000000000000",
    "000000000000000",
    "100000000111110",
    "000000000000000",
    "000000000000000",
    "000000000000000",
    "003400022220022",
    "005600000000000",
    "005600000000000",
    "005600000000000",
    "777777777777777"};

void Afficher(SDL_Renderer* afficheur, SDL_Texture* textuTil,
        char** table, int nombre_blocs_largeur, int nombre_blocs_hauteur)
{
    int i, j;
    SDL_Rect Rect_dest;
    SDL_Rect Rect_source;
    Rect_source.w = LARGEUR_TILE;
    Rect_dest.w   = LARGEUR_TILE;
    Rect_source.h = HAUTEUR_TILE;
    Rect_dest.h   = HAUTEUR_TILE;
    for(i = 0 ; i < NOMBRE_BLOCS_LARGEUR; i++)
    {
        for(j = 0 ; j < NOMBRE_BLOCS_HAUTEUR; j++)
        {
            Rect_dest.x = i * LARGEUR_TILE;
            Rect_dest.y = j * HAUTEUR_TILE;
            Rect_source.x = (table[j][i] - '0') * LARGEUR_TILE;
            Rect_source.y = 0;
            SDL_RenderCopy(afficheur, textuTil, &Rect_source, &Rect_dest);
        }
    }
}


int main(int argc, char *argv[])
{
    SDL_Window *ecran = NULL;
    SDL_Renderer *afficheur = NULL;

    SDL_Surface *tileset = NULL;
    SDL_Texture *textuTil = NULL;
    SDL_Event event;
    int continuer = 1, tempsPrecedent = 0, tempsActuel = 0;
    int statut = EXIT_FAILURE;


    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }

    ecran = SDL_CreateWindow("Kill Homer !", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            LARGEUR_TILE*NOMBRE_BLOCS_LARGEUR, HAUTEUR_TILE*NOMBRE_BLOCS_HAUTEUR, SDL_WINDOW_SHOWN);
    if (ecran == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }

    afficheur = SDL_CreateRenderer(ecran, -1, SDL_RENDERER_ACCELERATED);
    if (afficheur == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }

    tileset = IMG_Load("tileset1.bmp");
    if (tileset == NULL)
    {
        fprintf(stderr, "Erreur chargement image droite : %s", SDL_GetError());
        goto Quit;
    }

    textuTil = SDL_CreateTextureFromSurface(afficheur, tileset);
    if (textuTil == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateTexturetil : %s", SDL_GetError());
        goto Quit;
    }
    SDL_FreeSurface(tileset);

    Afficher(afficheur, textuTil, table, NOMBRE_BLOCS_LARGEUR, NOMBRE_BLOCS_HAUTEUR);
    while(continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        continuer = 0;
                        break;
                }
                break;
        }

        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent > 30) /* si 30ms se sont �coul� depuis le dernier tour de la boucle
                                                  plus le temps est petit plus il se deplace vite */
        {
            tempsPrecedent = tempsActuel; // le temps actuel devient le temps pr�sent
        }
        else
        {
            SDL_Delay(30 - (tempsActuel - tempsPrecedent));
        }

        SDL_RenderPresent(afficheur);

    }

Quit:
    if (NULL != textuTil)
        SDL_DestroyTexture(textuTil);
    if (NULL != afficheur)
        SDL_DestroyRenderer(afficheur);
    if (NULL != ecran)
        SDL_DestroyWindow(ecran);
    SDL_Quit();

    return statut;
}
