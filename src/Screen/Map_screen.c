#include <assert.h>
#include "SFML/Graphics.h"
#include "Gui/Gui.h"
#include "GraphicEngine/Image.h"
#include "Screen/Map_Screen.h"
#include "Objects/Editor.h"

unsigned long hash(unsigned char *str)
{
	unsigned long hash = 5381;
	while(*str!='\0') {
	        int c = *str;
                /* hash = hash*33 + c */
                hash = ((hash << 5) + hash) + c;
                str++;
        }
	return hash;
}

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
    if(image != NULL){
        sfSprite_SetImage(map->background, image);
        sfImage_SetSmooth(image, false);
    }
    sfSprite_SetPosition(map->background, x, y);

    assert(map->object = (Object***)malloc(3*sizeof(Object**)));
    for(int i = 0; i < 3; i++){
        map->object[i] = NULL;
        map->nombre_object[i] = 0;
    }

    map->editor = editor;

    return map;
}

void map_screen_Destroy(Map_screen* map)
{

    sfSprite_Destroy(map->background);

    for(int j = 0; j < 3; j++){
        for(int i = 0; i < map->nombre_object[j]; i++)
            object_Destroy(map->object[j][i]);

        free(map->object[j]);
        map->object[j] = NULL;
    }

    free(map);
    map = NULL;

}

void map_screen_Draw(Map_screen* map)
{

    sfRenderWindow_DrawSprite(map->Game, map->background);

    for(int j = 0; j < 3; j++)
        for(int i = 0; i < map->nombre_object[j]; i++)
            sfRenderWindow_DrawSprite(map->Game, map->object[j][i]->draw_img);

}


void map_screen_Click(Map_screen* map, int mouse_x, int mouse_y)
{

    sfIntRect cadre_screen = {map->x, map->y, map->x+map->largeur, map->y+map->hauteur};
    if (sfIntRect_Contains(&cadre_screen, mouse_x, mouse_y))
    {
        char temp_char[100];
        sprintf(temp_char, "%d%d%p%p", map->editor->object_create->type, map->editor->selected_type, sfSprite_GetImage(map->editor->selected_image), map->editor->selected_animation);

        int hash_object = hash(temp_char)%200;
        if(map->editor->object[hash_object] != NULL){

        }else{
            map->editor->object[hash_object] = object_Create();
            map->editor->object[hash_object]->type = map->editor->object_create->type;
            map->editor->object[hash_object]->bool_animation = map->editor->selected_type;
            sfSprite_SetImage(map->editor->object[hash_object]->draw_img, sfSprite_GetImage(map->editor->selected_image));
            map->editor->object[hash_object]->draw_animation = map->editor->selected_animation;
        }

        Object* obj = object_Create();
        obj->type = map->editor->object_create->type;
        obj->bool_animation = map->editor->selected_type;
        sfSprite_SetImage(obj->draw_img, sfSprite_GetImage(map->editor->selected_image));
        sfSprite_SetPosition(obj->draw_img, mouse_x, mouse_y);
        obj->draw_animation = map->editor->selected_animation;

        map_screen_AddObj(map, map->editor->current_plan, obj);
    }

}


void map_screen_AddObj(Map_screen* map, int plan, Object* obj)
{

    map->nombre_object[plan]++;
    assert(map->object[plan] = realloc(map->object[plan], map->nombre_object[plan]*sizeof(Object*)));
    map->object[plan][map->nombre_object[plan]-1] = obj;

}


