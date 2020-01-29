#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY "276326311"
#define EXT "txt"

char * changerExt(char * nomDoc){
    int i;
    char *nom;

    for(i = strlen(nomDoc); i >= 0 && nomDoc[i] != '.'; i--);

    nom = malloc(sizeof (*nom) * (i + strlen(EXT)));
    for(int j = 0; j <= i; j++)
        nom[j] = nomDoc[j];

    nom[i+1] = '\0';

    strcat(nom, EXT);

    return nom;
    
}

int main(int argc, char *argv[]){
    
    FILE * doc = NULL, * res = NULL;
    char *nomDoc = malloc(sizeof (*nomDoc) * 50);
    char *nomRes;
    char c;
    int lnK = strlen(KEY), i = 0;
    
    if(argc>1){
        doc = fopen(argv[1],"r");
        if(!doc){
            free(nomDoc);
            return 1;
        }
        nomRes = changerExt(argv[1]);
        res = fopen(nomRes, "w");
    }else{
        printf("Saisir le nom du fichier a traiter: ");
        scanf("%s", nomDoc);
        doc = fopen(nomDoc,"r");
        if(!doc){
            free(nomDoc);
            return 1;
        }
        nomRes = changerExt(nomDoc);
        res = fopen(nomRes, "w");
    }

    fscanf(doc, "%c", &c);
    while(!feof(doc)){
        if(c != ' ' && c != '\n'){
            fprintf(res, "%c", c - KEY[i%lnK]);
            i++;
        }else
            fprintf(res, "%c", c);
        fscanf(doc, "%c", &c);
    }

    printf("\nLa salle a ete decodee dans %s\n", nomRes);

    fclose(doc);
    fclose(res);
    free(nomRes);
    free(nomDoc);

    system("PAUSE");
    
    return 0;
}