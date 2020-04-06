#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
    #define PAUSE system("PAUSE");
    #define CLEAR system("CLS");
#elif defined(__unix)
    #define PAUSE system("read -rsp $'Press any key to continue...\n' -n 1");
    #define CLEAR system("clear");
#endif

/* Informations non ajoutées à la doc dans un but de protection */
#define KEY "276326311"
#define EXT "txt"

/**
 * \brief Change l'extension du nom du document (de EXT -> txt)
 *
 * @param nomDoc Nom du document
 * @return adresse vers le nouveau nom du document
*/
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

/**
 * \brief
 *
 * @param argv argument du main(facultatif): nom du document à traiter
 * @return code erreur: 0 si tout s'est bien passé.
*/
int main(int argc, char *argv[]){

    FILE * doc = NULL,/**< Document d'entré */ * res = NULL; /**< Document de sorti */
    char *nomDoc = malloc(sizeof (*nomDoc) * 50); /**< Nom du document à ouvrir */
    char *nomRes; /**< Nom du document résultat */
    char c; /**< Caractère buffer: sert à traiter le fichier caractère par caractère */
    int lnK = strlen(KEY), i = 0;

    //Ouverture du doc et création du doc res
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

    //Traitement du fichier
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

    //Libération de la mémoire
    fclose(doc);
    fclose(res);
    free(nomRes);
    free(nomDoc);

    PAUSE

    return 0;
}
