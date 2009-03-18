#ifndef OBJECTSSCREEN_H
#define OBJECTSSCREEN_H

#include "SFML/Graphics.h"
#include "Objects/GameObjects.h"

typedef struct OBJECT_SCREEN
{

    sfRenderWindow* Game;

    sfSprite* background;

    Image* image;

    Animation** animation;
    int animation_nombre;

    int type_affichage; //0 = image, 1 = animation

    int x;
    int y;
    int largeur;
    int hauteur;

    int x_cur;
    int y_cur;
    int y_max;
    int espace;

    int y_pos;


}Object_Screen;

Object_Screen* object_screen_Create(sfRenderWindow*, sfImage*, int, int, int, int);
void object_screen_Destroy(Object_Screen*);
void object_screen_Draw(Object_Screen*);
void object_screen_Load_Object(Object_Screen*, Image*); // Load sans copie et malloc, il faut donc un tableau d'object qui existe
void object_screen_Load_Animation(Object_Screen*, Animation**, int); // Load sans copie et malloc, il faut donc un tableau d'object qui existe (mais avec destruction)
int object_screen_Click(Object_Screen* screen, int mouse_x, int mouse_y);

#endif
