/*
    GPL v3 Licence :
    Bombersow is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bombersow is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bombersow.  If not, see <http://www.gnu.org/licenses/>.


    Creative Commons BY-NC-SA :
    This work is licensed under the Creative Commons Attribution-Noncommercial-Share Alike 3.0 Unported License.
    To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter
    to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.

*/

#include <assert.h>
#include "SFML/Graphics.h"
#include "Gui/Gui.h"
#include "GraphicEngine/Image.h"
#include "Screen/Map_screen.h"
#include "Objects/Editor.h"

unsigned long hash(char *str)
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
            object_Draw(map->Game, map->object[j][i]);

}

void map_screen_DrawPlan(Map_screen* map, int plan)
{
    if(plan == 0)
        sfRenderWindow_DrawSprite(map->Game, map->background);

    for(int i = 0; i < map->nombre_object[plan]; i++){

        object_Draw(map->Game, map->object[plan][i]);

        if(map->object[plan][i]->speed != 0){
            sprite_SetColor(map->object[plan][i]->sprite, sfColor_FromRGBA(255,255,255,120));
            sprite_SetPosition(map->object[plan][i]->sprite, map->object[plan][i]->dest_coord_x,map->object[plan][i]->dest_coord_y);
            sprite_Draw(map->Game, map->object[plan][i]->sprite);
            sprite_SetColor(map->object[plan][i]->sprite, sfColor_FromRGBA(255,255,255,255));
            sprite_SetPosition(map->object[plan][i]->sprite, map->object[plan][i]->curr_coord_x, map->object[plan][i]->curr_coord_y);
        }
    }

}


void map_screen_Click(Map_screen* map, int mouse_x, int mouse_y)
{

    sfIntRect cadre_screen = {map->x, map->y, map->x+map->largeur, map->y+map->hauteur};
    if (sfIntRect_Contains(&cadre_screen, mouse_x, mouse_y))
    {

        if(map->editor->dynamic_step == 1){

            map->editor->object_create_dynamic->dest_coord_x = map->editor->selected_sprite->x;
            map->editor->object_create_dynamic->dest_coord_y = map->editor->selected_sprite->y;

            map->editor->object_create_dynamic->speed = map->editor->object_create->speed;

            sfClock_Reset(map->editor->object_create_dynamic->clock_mouvement);

            map->editor->object_create_dynamic = NULL;
            map->editor->dynamic_step = 0;
        }else{

            char temp_char[100];
            sprintf(temp_char, "%d%d%p", map->editor->object_create->type, map->editor->selected_type, sprite_GetImage(map->editor->selected_sprite));

            int hash_object = hash(temp_char)%200;
            if(map->editor->object[hash_object] != NULL){
                if(sprite_GetImage(map->editor->selected_sprite) != sprite_GetImage(map->editor->object[hash_object]->sprite)){
                    for(int i = 0; i < 200; i++){
                        if( sprite_GetImage(map->editor->selected_sprite) == sprite_GetImage(map->editor->object[hash_object]->sprite) ){
                            hash_object = i;
                        }
                    }
                }

            }else{
                map->editor->object[hash_object] = object_Create(0, PLATFORM, 0, 0, NULL, NULL);
                map->editor->object[hash_object]->type = map->editor->object_create->type;
                Animation* temp_anim = NULL;
                if(map->editor->selected_sprite->type)
                    temp_anim = animation_Create(
                            sprite_GetImage(map->editor->selected_sprite),
                            map->editor->selected_sprite->animation->x,
                            map->editor->selected_sprite->animation->y,
                            map->editor->selected_sprite->animation->image_hauteur,
                            map->editor->selected_sprite->animation->image_largeur,
                            map->editor->selected_sprite->animation->nombre_image,
                            0, BOUCLE,
                            map->editor->selected_sprite->animation->fps);
                map->editor->object[hash_object]->sprite = sprite_Create(map->editor->selected_sprite->x,map->editor->selected_sprite->y, sprite_GetImage(map->editor->selected_sprite), temp_anim);
            }

            Object* obj = object_Create(0, PLATFORM, 0, 0, NULL, NULL);
            obj->type = map->editor->object_create->type;
            obj->speed = 0;
            obj->weapon_id = map->editor->object_create->weapon_id;
            obj->nb_ammo = map->editor->object_create->nb_ammo;

            obj->start_coord_x = map->editor->selected_sprite->x;
            obj->start_coord_y = map->editor->selected_sprite->y;

            obj->curr_coord_x = obj->start_coord_x;
            obj->curr_coord_y = obj->start_coord_y;


            if(map->editor->selected_sprite->type){
                Animation* temp_anim = NULL;
                temp_anim = animation_Create(
                                            sprite_GetImage(map->editor->selected_sprite),
                                            map->editor->selected_sprite->animation->x,
                                            map->editor->selected_sprite->animation->y,
                                            map->editor->selected_sprite->animation->image_hauteur,
                                            map->editor->selected_sprite->animation->image_largeur,
                                            map->editor->selected_sprite->animation->nombre_image,
                                            0, BOUCLE,
                                            map->editor->selected_sprite->animation->fps);

                object_LoadImg(obj, NULL, temp_anim);

            }else{
                object_LoadImg(obj, sprite_GetImage(map->editor->selected_sprite), NULL);
            }

            obj->objectID = hash_object;

            if(map->editor->bool_dynamic && map->editor->dynamic_step == 0){
                map->editor->dynamic_step = 1;
                map->editor->object_create_dynamic = obj;
            }

            map_screen_AddObj(map, map->editor->current_plan, obj);
        }
    }

}


void map_screen_AddObj(Map_screen* map, int plan, Object* obj)
{

    map->nombre_object[plan]++;
    assert(map->object[plan] = realloc(map->object[plan], map->nombre_object[plan]*sizeof(Object*)));
    map->object[plan][map->nombre_object[plan]-1] = obj;

}


