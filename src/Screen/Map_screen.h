#ifndef MAPSCREEN_H
#define MAPSCREEN_H

#include "SFML/Graphics.h"
#include "Gui/Gui.h"
#include "Objects/Editor.h"
#include "Objects/GameObjects.h"

typedef struct MAP_SCREEN
{

    sfRenderWindow* Game;

    sfSprite* background;

    Object*** object;
    int nombre_object[3];

    int x;
    int y;
    int largeur;
    int hauteur;

    Editor* editor;

}Map_screen;

Map_screen* map_screen_Create(sfRenderWindow*, sfImage*, Editor*, int, int, int, int);
void map_screen_Destroy(Map_screen*);
void map_screen_Draw(Map_screen*);

void map_screen_Click(Map_screen*, int, int);

void map_screen_AddObj(Map_screen*, int, Object*);

#endif
