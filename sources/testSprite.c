#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../headers/source.h"

int main(){

    char* res = chercherSprite(301,"./sprites/");

    printf("%s\n",res);

    free(res);

    return 0;
}