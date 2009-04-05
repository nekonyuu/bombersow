#include "BaseSystem/Logging.h"
#include "Objects/GameObjects.h"

Object* object_Create(unsigned int type)
{
    Object* new_object = NULL;
    assert(new_object = (Object*) malloc(sizeof(Object)));

    new_object->objectID = 0;
    new_object->type = type;

    new_object->draw_img = sfSprite_Create();
    new_object->draw_animation = NULL;
    new_object->bool_animation = 0;

    new_object->start_coord_x = 0;
    new_object->start_coord_y = 0;

    new_object->curr_coord_x = 0;
    new_object->curr_coord_y = 0;

    new_object->dest_coord_x = 0;
    new_object->dest_coord_y = 0;

    new_object->speed = 0;
    new_object->weapon_id = 0;
    new_object->nb_ammo = 0;

    new_object->clock_mouvement = sfClock_Create();

    new_object->spawned = true;

    return new_object;
}

void object_Destroy(Object* object2destroy)
{
    if(object2destroy != NULL){
        sfClock_Destroy(object2destroy->clock_mouvement);

        if(object2destroy->draw_img != NULL)
            sfSprite_Destroy(object2destroy->draw_img);

        animation_Destroy(object2destroy->draw_animation);
        free(object2destroy);
    }
}

void object_LoadImg(Object* object, sfImage* image, Animation* animation)
{

    if(image == NULL){
        object->bool_animation = 1;
        object->draw_animation = animation;
        animation_SetPosition(object->draw_animation, (int)object->curr_coord_x, (int)object->curr_coord_y);
    }else{
        object->bool_animation = 0;
        sfSprite_SetImage(object->draw_img, image);
        sfSprite_SetPosition(object->draw_img, (int)object->curr_coord_x, (int)object->curr_coord_y);
    }

}

void object_Draw(sfRenderWindow* Game, Object* object)
{

    if(object->speed != 0){
        if(object->start_coord_x < object->dest_coord_x && object->curr_coord_x < object->dest_coord_x)
            object->curr_coord_x += (float)object->speed*sfClock_GetTime(object->clock_mouvement);

        if(object->start_coord_x > object->dest_coord_x && object->curr_coord_x > object->dest_coord_x)
            object->curr_coord_x -= (float)object->speed*sfClock_GetTime(object->clock_mouvement);

        if(object->start_coord_y < object->dest_coord_y && object->curr_coord_y < object->dest_coord_y)
            object->curr_coord_y += (float)object->speed*sfClock_GetTime(object->clock_mouvement);

        if(object->start_coord_y > object->dest_coord_y && object->curr_coord_y > object->dest_coord_y)
            object->curr_coord_y -= (float)object->speed*sfClock_GetTime(object->clock_mouvement);

        float start_x = abs(object->dest_coord_x - object->start_coord_x);
        float curr_x = abs(object->curr_coord_x - object->start_coord_x);

        float start_y = abs(object->dest_coord_y - object->start_coord_y);
        float curr_y = abs(object->curr_coord_y - object->start_coord_y);

        if(curr_x >= start_x && curr_y >= start_y){
            start_x = object->start_coord_x;
            object->start_coord_x = object->dest_coord_x;
            object->dest_coord_x = start_x;

            start_y = object->start_coord_y;
            object->start_coord_y = object->dest_coord_y;
            object->dest_coord_y = start_y;
        }
        sfClock_Reset(object->clock_mouvement);

        if(object->bool_animation){
            animation_SetPosition(object->draw_animation, (int)object->curr_coord_x, (int)object->curr_coord_y);
        }else{
            sfSprite_SetPosition(object->draw_img, (int)object->curr_coord_x, (int)object->curr_coord_y);
        }
    }

    if(object->bool_animation){
        animation_Draw(object->draw_animation, Game);
    }else{
        sfRenderWindow_DrawSprite(Game, object->draw_img);
    }

}
