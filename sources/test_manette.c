#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main (int argc, char* argv[])
{
     // D�marre SDL avec la gestion des joysticks
  if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == -1 )
  {
    fprintf(stderr,"Erreur lors de l'initialisation de la SDL\n");
    return -1;
  }
SDL_Joystick* pJoystick = NULL;
int numJoystick = SDL_NumJoysticks(); // Compte le nombre de joysticks
printf("Vous avez %d joysticks sur cette machine\n",numJoystick);
if ( numJoystick >= 1 )
{
    // Ouvre le premier joystick pr�sent sur la machine
    pJoystick = SDL_JoystickOpen(0);
    if ( pJoystick == NULL )
    {
        fprintf(stderr,"Erreur pour ouvrir le premier joystick\n");
    }
}

while (1) {
SDL_Event event;
SDL_WaitEvent(&event);
if ( event.type == SDL_JOYBUTTONDOWN )
{
    printf("Bouton : %d\n",event.jbutton.button);
}

// On indique que l'on souhaite utiliser le syst�me d'�v�nements
SDL_JoystickEventState(SDL_ENABLE);

if ( event.type == SDL_JOYAXISMOTION )
{
    // Mouvement d'un axe
    // Nous devons donc utiliser le champ jaxis
    printf("Mouvement d'un axe\n");
    printf("%d est la nouvelle valeur de l'axe %d pour le joystick %d\n",
           event.jaxis.value,
           event.jaxis.axis,
           event.jaxis.which);
}
else if ( event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYBUTTONUP )
{
    // Bouton appuy� ou rel�ch�
    // Nous devons donc utiliser le champ jbutton
    if ( event.jbutton.state == SDL_PRESSED )
    {
        printf("Appui sur le bouton %d du joystick %d\n",
                event.jbutton.button,
                event.jbutton.which);
    }
    else if ( event.jbutton.state == SDL_RELEASED )
    {
        printf("Rel�chement sur le bouton %d du joystick %d\n",
                event.jbutton.button,
                event.jbutton.which);
    }
}
else if ( event.type == SDL_JOYBALLMOTION )
{
    // Mouvement de trackball
    // Nous devons donc utiliser le champ jball
    printf("Mouvement du trackball\n");
    printf("La balle %d du joystick %d a boug� de %d;%d\n",
           event.jball.ball,
           event.jball.which,
           event.jball.xrel,
           event.jball.yrel);
}
else if ( event.type == SDL_JOYHATMOTION )
{
    // Mouvement d'un chapeau
    // Nous devons donc utiliser le champ jhat
    printf("Mouvement du chapeau %d du joystick %d\n",event.jhat.hat,event.jhat.which);
    if ( event.jhat.value == SDL_HAT_CENTERED )
    {
        printf("Retour position neutre\n");
    }
    if ( event.jhat.value == SDL_HAT_DOWN )
    {
        printf("Position bas\n");
    }
    if ( event.jhat.value == SDL_HAT_LEFT )
    {
        printf("Position gauche\n");
    }
    if ( event.jhat.value == SDL_HAT_RIGHT )
    {
        printf("Position droite\n");
    }
    if ( event.jhat.value == SDL_HAT_UP )
    {
        printf("Position haut\n");
    }
}
    }
}
