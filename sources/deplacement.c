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

    int tab_touche[5] = {0,0,0,0,0};
    int continuer = 0;
    int touche;
    /* infinite loop */
    while (continuer!=10) {
      while(kbhit()){
        touche = getch();

        switch(touche){
          case 	KEY_LEFT :
            continuer++;
            tab_touche[0]++;
            break;
          case KEY_RIGHT :
            continuer++;
            tab_touche[2]++;
            break;
          case 	KEY_UP :
            continuer++;
            tab_touche[1]++;
            break;
          case KEY_DOWN :
            continuer++;
            tab_touche[3]++;
            break;
          case 224 :
            break;
          default :
            continuer++;
            tab_touche[4]++;
            break;
        }
      }
    }

    for(int i = 0; i<5; i++)
      printf("%d \n", tab_touche[i]); 

}
