#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define KEY_UP      72
#define KEY_DOWN    80
#define KEY_LEFT    75
#define KEY_RIGHT   77

int main(void)
{
  int tab_touche[5][5] = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
  tab_touche[4][0] = 1;
  int touche;
  int continuer = 1;
  int i = 4, j = 0;

    while(continuer != 0){
      while(kbhit()){
          touche = getch();

          switch(touche){
            case 	KEY_LEFT :
              if(j!=0){
                tab_touche[i][j] = 0;
                j--;
                tab_touche[i][j] = 1;
              }
              else
                printf("deplacement impossible, bord de la carte");
              break;
            case KEY_RIGHT :
              if(j!=4){
                tab_touche[i][j] = 0;
                j++;
                tab_touche[i][j] = 1;
              }
              else
                printf("deplacement impossible, bord de la carte");
              break;
            case 	KEY_UP :
              if(i!=0){
                tab_touche[i][j] = 0;
                i--;
                tab_touche[i][j] = 1;
              }
              else
                printf("deplacement impossible, bord de la carte");
              break;
            case KEY_DOWN :
              if(i!=4){
                tab_touche[i][j] = 0;
                i++;
                tab_touche[i][j] = 1;
              }
              else
                printf("deplacement impossible, bord de la carte");
              break;
            case 224 :
              break;
            default :
              continuer = 0;
              break;
          }
    }

    for(i=0; i<5; i++){
      for(j=0; j<5; j++){
        printf("%d ", tab_touche[i][j]);
      }
      printf("\n");
    }
  }

}
