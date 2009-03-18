#include <assert.h>
#include "SFML/Graphics.h"
#include "Gui/Gui.h"
#include "GraphicEngine/Image.h"
#include "Screen/Map_Screen.h"
#include "Objects/Editor.h"


Map_screen* map_screen_Create(sfRenderWindow* Game, sfImage* image, Editor* editor, int x, int y, int largeur, int hauteur)
{

    Map_screen* map = NULL;
    assert(map = (Map_screen*)malloc(sizeof(Map_screen)));

    map->x = x;
    map->y = y;

    map->largeur = largeur;
    map->hauteur = hauteur;

    map->Game = Game;

    map->background = sfSprite_Create();
    sfSprite_SetImage(map->background, image);
    sfImage_SetSmooth(image, false);
    sfSprite_SetPosition(map->background, x, y);

    map->object_0 = NULL;
    map->nombre_object_0 = 0;

    map->object_1 = NULL;
    map->nombre_object_1 = 0;

    map->object_2 = NULL;
    map->nombre_object_2 = 0;

    return map;
}

void map_screen_Destroy(Map_screen* map)
{

    sfSprite_Destroy(map->background);

    for(int i = 0; i < map->nombre_object_0; i++)
        object_Destroy(map->object_0[i]);

    free(map->object_0);
    map->object_0 = NULL;

    for(int i = 0; i < map->nombre_object_1; i++)
        object_Destroy(map->object_1[i]);

    free(map->object_1);
    map->object_1 = NULL;

    for(int i = 0; i < map->nombre_object_2; i++)
        object_Destroy(map->object_2[i]);

    free(map->object_2);
    map->object_2 = NULL;

    free(map);
    map = NULL;

}

void map_screen_Draw(Map_screen* map)
{

    sfRenderWindow_DrawSprite(map->Game, map->background);

    for(int i = 0; i < map->nombre_object_0; i++)
        sfRenderWindow_DrawSprite(map->Game, map->object_0[i]->draw_img);

    for(int i = 0; i < map->nombre_object_1; i++)
        sfRenderWindow_DrawSprite(map->Game, map->object_1[i]->draw_img);

    for(int i = 0; i < map->nombre_object_2; i++)
        sfRenderWindow_DrawSprite(map->Game, map->object_2[i]->draw_img);

}


void map_screen_Click(Map_screen* map, int mouse_x, int mouse_y)
{

    sfIntRect cadre_screen = {map->x, map->y, map->x+map->largeur, map->y+map->hauteur};
    if (sfIntRect_Contains(&cadre_screen, mouse_x, mouse_y))
    {

    }

}


