#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../headers/source.h"

int main(int argc, char* argv[]){

    if(argc != 2)
      return 1;

    int nb = atoi(argv[1]);

    char* res = chercherSprite(nb,"./sprites/");

    printf("Resultat: %s\n", res ? res : "air");

    if(res && strcmp(res,BLOC_ERR))
      free(res);

    return 0;
}
