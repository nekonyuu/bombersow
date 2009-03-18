#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <SFML/Graphics.h>
#include "Gui/Gui.h"
#include "Objects/Editor.h"
#include "Objects/GameObjects.h"

typedef enum MENU_IMAGE {MENU_BACKGROUND, MENU_TEXTBOX_BACKGROUND, MENU_BOUTON}Menu_image;
typedef struct OBJECT_MENU
{

    sfRenderWindow* Game;

    sfSprite* background;

    Image* image;

    int x;
    int y;
    int largeur;
    int hauteur;

    Editor* editor;

    Gui* gui;

}Object_Menu;

Object_Menu* menu_screen_Create(sfRenderWindow*, Image*, int, int, int, int, Editor*);
void menu_screen_Destroy(Object_Menu* menu);
void menu_screen_Draw(Object_Menu* menu);
void menu_screen_Click(Object_Menu* menu, int mouse_x, int mouse_y);
void menu_screen_MouseOver(Object_Menu* menu, int mouse_x, int mouse_y);

#endif
