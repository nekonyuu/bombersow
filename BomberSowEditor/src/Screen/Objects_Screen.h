#ifndef OBJECTSSCREEN_H
#define OBJECTSSCREEN_H

#include "SFML/Graphics.h"
#include "Objects/Objects.h"

typedef struct OBJECT_SCREEN{

    sfRenderWindow* Game;

    sfSprite* background;

    Image* image;

    int x;
    int y;
    int largeur;
    int hauteur;

    int x_cur;
    int y_cur;
    int y_max;
    int espace;

    int page;
    int last_id;
    int first_id;

}Object_Screen;

Object_Screen* object_screen_Create(sfRenderWindow*, sfImage*, int, int, int, int);
void object_screen_Destroy(Object_Screen*);
void object_screen_Draw(Object_Screen*);
void object_screen_Load_Object(Object_Screen*, Image*); // Load sans copie et malloc, il faut donc un tableau d'object qui existe
int object_screen_Click(Object_Screen* screen, int mouse_x, int mouse_y);

#endif
