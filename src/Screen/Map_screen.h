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

    Object** object_0;
    int nombre_object_0;

    Object** object_1;
    int nombre_object_1;

    Object** object_2;
    int nombre_object_2;

    int x;
    int y;
    int largeur;
    int hauteur;

}Map_screen;

Map_screen* map_screen_Create(sfRenderWindow*, sfImage*, Editor*, int, int, int, int);
void map_screen_Destroy(Map_screen*);
void map_screen_Draw(Map_screen*);

void map_screen_Click(Map_screen*, int, int);

#endif
