#include <stdio.h>
#include <stdlib.h>

#define X 5
#define Y 5

/**
 * \enum direction
 * \brief Premet l'utilisation des 4 directions dans les fonctions
*/
typedef enum {BAS, HAUT, GAUCHE, DROITE} direction;


/**
 * \brief Deplace le joueur depuis sa position selon la direction
 *
 * @param tab tableau où se trouve le point à déplacer
 * @param x indice d'abscisse où se trouve l'element
 * @param y indice d'ordonnée où se trouve l'element
 * @param direction direction vers laquelle bouge l'element (type enum)
 */

void deplacer(int * tab, int x, int y, direction direction){
  switch(direction){
    case 	HAUT :
      if(y>0){
        *(tab+(Y*x+y)) = 0;
        *(tab+(Y*x+y-1)) = 1;
      }
      break;
    case BAS :
      if(y<Y-1){
        *(tab+(Y*x+y)) = 0;
        *(tab+(Y*x+y+1)) = 1;
      }
      break;
    case 	GAUCHE :
      if(x>0){
        *(tab+(Y*x+y)) = 0;
        *(tab+(Y*(x-1)+y)) = 1;
      }
      break;
    case DROITE :
      if(x<X-1){
        *(tab+(Y*x+y)) = 0;
        *(tab+(Y*(x+1)+y)) = 1;
      }
      break;
  }
}

/**
 * \brief Affiche la matrice
 *
 * @param tab matrice à afficher
 */

void afficher_mat(int * tab){
  for(int x=0; x<X; x++){
    for(int y=0; y<Y; y++){
      printf("%i ", *(tab+(Y*x+y)));
    }
    printf("\n");
  }
}

/**
 * \brief Reinitialise la matrice à 0
 *
 * @param tab matrice à reinitialiser
 */

void clean_mat(int * tab){
  for(int x=0; x<X; x++){
    for(int y=0; y<Y; y++){
      *(tab+(Y*x+y))=0;
    }
  }
}

/**
 * \brief Fait se déplacer un point dans toutes les directions
 *
 * @param tab tableau où se trouve le point à déplacer
 * @param x indice d'abscisse où se trouve l'element
 * @param y indice d'ordonnée où se trouve l'element
 */

void test_direction(int * tab, int x, int y){
  for(int dir=BAS; dir<=DROITE; dir++){
    *(tab+(Y*x+y)) = 1; //Positionnement initial
    afficher_mat(tab); //Affichage état initial
    printf("Direction : %i\n", dir);
    deplacer(tab, x, y, dir); //Déplacement dans une direction
    afficher_mat(tab); //Affichage après déplacement
    clean_mat(tab); //Remise à zéro
    printf("\n");
  }
}

/**
 * \brief Test de déplacement dans toutes les directions à plusieurs endroits clés
 *
 * @return 0 si tout s'est bien déroulé
 */
int main(){
  //Programme de test de la fonction de déplacement
  int x=0;
  int y=0;
  int * tab;
  tab=malloc(X*Y*sizeof(int));
  clean_mat(tab);
  //Cas en haut à gauche
  test_direction(tab, x, y);
  printf("\n\n");
  //Cas en bas à gauche
  x=X-1;
  y=0;
  test_direction(tab, x, y);
  printf("\n\n");
  //Cas en haut à droite
  x=0;
  y=Y-1;
  test_direction(tab, x, y);
  printf("\n\n");
  //Cas en bas à droite
  x=X-1;
  y=Y-1;
  test_direction(tab, x, y);
  printf("\n\n");
  //Cas en haut
  x=0;
  y=1;
  test_direction(tab, x, y);
  printf("\n\n");
  //Cas en bas
  x=X-1;
  y=1;
  test_direction(tab, x, y);
  printf("\n\n");
  //Cas à gauche
  x=1;
  y=0;
  test_direction(tab, x, y);
  printf("\n\n");
  //Cas à droite
  x=1;
  y=Y-1;
  test_direction(tab, x, y);
  printf("\n\n");
  //Cas quelconque
  x=1;
  y=1;
  test_direction(tab, x, y);
  printf("\n\n");
  free(tab);
  return 0;
}
