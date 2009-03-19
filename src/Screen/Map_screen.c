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

void map_screen_DrawPlan(Map_screen* map, int plan)
{
    if(plan == 0)
        sfRenderWindow_DrawSprite(map->Game, map->background);

    for(int i = 0; i < map->nombre_object[plan]; i++){

        object_Draw(map->Game, map->object[plan][i]);

        if(map->object[plan][i]->speed != 0){
            if(map->object[plan][i]->draw_animation != NULL){
                sfSprite_SetColor(map->object[plan][i]->draw_animation->sprite, sfColor_FromRGBA(255,255,255,120));
                animation_SetPosition(map->object[plan][i]->draw_animation, map->object[plan][i]->dest_coord_x,map->object[plan][i]->dest_coord_y);
                animation_Draw(map->object[plan][i]->draw_animation, map->Game);
                sfSprite_SetColor(map->object[plan][i]->draw_animation->sprite, sfColor_FromRGBA(255,255,255,255));
                animation_SetPosition(map->object[plan][i]->draw_animation, map->object[plan][i]->curr_coord_x, map->object[plan][i]->curr_coord_y);
            }else{
                sfSprite_SetColor(map->object[plan][i]->draw_img, sfColor_FromRGBA(255,255,255,120));
                sfSprite_SetPosition(map->object[plan][i]->draw_img, map->object[plan][i]->dest_coord_x,map->object[plan][i]->dest_coord_y);
                sfRenderWindow_DrawSprite(map->Game, map->object[plan][i]->draw_img);
                sfSprite_SetColor(map->object[plan][i]->draw_img, sfColor_FromRGBA(255,255,255,255));
                sfSprite_SetPosition(map->object[plan][i]->draw_img, map->object[plan][i]->curr_coord_x, map->object[plan][i]->curr_coord_y);
            }
        }
    }

}


void map_screen_Click(Map_screen* map, int mouse_x, int mouse_y)
{

    sfIntRect cadre_screen = {map->x, map->y, map->x+map->largeur, map->y+map->hauteur};
    if (sfIntRect_Contains(&cadre_screen, mouse_x, mouse_y))
    {

        if(map->editor->dynamic_step == 1){

            if(map->editor->selected_type){
                map->editor->object_create_dynamic->dest_coord_x = map->editor->selected_animation->x_c;
                map->editor->object_create_dynamic->dest_coord_y = map->editor->selected_animation->y_c;
            }else{
                map->editor->object_create_dynamic->dest_coord_x = sfSprite_GetX(map->editor->selected_image);
                map->editor->object_create_dynamic->dest_coord_y = sfSprite_GetY(map->editor->selected_image);
            }
            map->editor->object_create_dynamic->speed = map->editor->object_create->speed;

            sfClock_Reset(map->editor->object_create_dynamic->clock_mouvement);

            map->editor->object_create_dynamic = NULL;
            map->editor->dynamic_step = 0;
        }else{

            char temp_char[100];
            sprintf(temp_char, "%d%d%p%p", map->editor->object_create->type, map->editor->selected_type, sfSprite_GetImage(map->editor->selected_image), map->editor->selected_animation);

            int hash_object = hash(temp_char)%200;
            if(map->editor->object[hash_object] != NULL){
                if(sfSprite_GetImage(map->editor->selected_image) != sfSprite_GetImage(map->editor->object[hash_object]->draw_img) || map->editor->object[hash_object]->draw_animation != map->editor->selected_animation){
                    for(int i = 0; i < 200; i++){
                        if( (sfSprite_GetImage(map->editor->selected_image) == sfSprite_GetImage(map->editor->object[hash_object]->draw_img) || map->editor->object[hash_object]->draw_animation == map->editor->selected_animation) || map->editor->object[i] == NULL){
                            hash_object = i;
                        }
                    }
                }

            }

            if(map->editor->object[hash_object] == NULL){

                map->editor->object[hash_object] = object_Create();
                map->editor->object[hash_object]->type = map->editor->object_create->type;
                map->editor->object[hash_object]->bool_animation = map->editor->selected_type;
                sfSprite_SetImage(map->editor->object[hash_object]->draw_img, sfSprite_GetImage(map->editor->selected_image));

                map->editor->object[hash_object]->draw_animation = map->editor->selected_animation;
            }

            Object* obj = object_Create();
            obj->type = map->editor->object_create->type;
            obj->bool_animation = map->editor->selected_type;
            obj->speed = 0;
            obj->weapon_id = map->editor->object_create->weapon_id;
            obj->nb_ammo = map->editor->object_create->nb_ammo;

            obj->start_coord_x = sfSprite_GetX(map->editor->selected_image);
            obj->start_coord_y = sfSprite_GetY(map->editor->selected_image);

            obj->curr_coord_x = obj->start_coord_x;
            obj->curr_coord_y = obj->start_coord_y;

            if(obj->bool_animation == 1){
                obj->draw_animation = animation_Create(
                                            sfSprite_GetImage(map->editor->selected_animation->sprite),
                                            map->editor->selected_animation->x,
                                            map->editor->selected_animation->y,
                                            map->editor->selected_animation->image_hauteur,
                                            map->editor->selected_animation->image_largeur,
                                            map->editor->selected_animation->nombre_image,
                                            0, BOUCLE,
                                            map->editor->selected_animation->fps);

                obj->curr_coord_x = map->editor->selected_animation->x_c;
                obj->curr_coord_y = map->editor->selected_animation->y_c;

                obj->start_coord_x = map->editor->selected_animation->x_c;
                obj->start_coord_y = map->editor->selected_animation->y_c;

                object_LoadImg(obj, NULL, obj->draw_animation);
            }else{
                object_LoadImg(obj, sfSprite_GetImage(map->editor->selected_image), NULL);
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


