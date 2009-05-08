#include "BaseSystem/Logging.h"
#include "Objects/GameObjects.h"

Object* object_Create(unsigned int type)
{
    Object* new_object = NULL;
    assert(new_object = (Object*) malloc(sizeof(Object)));

    new_object->objectID = 0;
    new_object->type = type;

    new_object->sprite = NULL;

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

    new_object->spawned = sfTrue;

    new_object->quad_node = NULL;
    new_object->list_node = NULL;

    new_object->gravity = 0;

    return new_object;
}

void object_Destroy(Object* object2destroy)
{
    if (!object2destroy)
        logging_Warning("object_Destroy", "Object object sent NULL");
    else
    {
        sfClock_Destroy(object2destroy->clock_mouvement);
        sprite_Destroy(object2destroy->sprite);
        free_secure(object2destroy);
    }
}

void object_LoadImg(Object* object, sfImage* image, Animation* animation)
{
    sprite_Destroy(object->sprite);
    object->sprite = sprite_Create((int)object->curr_coord_x, (int)object->curr_coord_y, image, animation);
}

void object_Draw(sfRenderWindow* Game, Object* object)
{

    if (object->speed != 0)
    {
        if (object->start_coord_x < object->dest_coord_x && object->curr_coord_x < object->dest_coord_x)
            object->curr_coord_x += (float)object->speed*sfClock_GetTime(object->clock_mouvement);

        if (object->start_coord_x > object->dest_coord_x && object->curr_coord_x > object->dest_coord_x)
            object->curr_coord_x -= (float)object->speed*sfClock_GetTime(object->clock_mouvement);

        if (object->start_coord_y < object->dest_coord_y && object->curr_coord_y < object->dest_coord_y)
            object->curr_coord_y += (float)object->speed*sfClock_GetTime(object->clock_mouvement);

        if (object->start_coord_y > object->dest_coord_y && object->curr_coord_y > object->dest_coord_y)
            object->curr_coord_y -= (float)object->speed*sfClock_GetTime(object->clock_mouvement);

        float start_x = abs(object->dest_coord_x - object->start_coord_x);
        float curr_x = abs(object->curr_coord_x - object->start_coord_x);

        float start_y = abs(object->dest_coord_y - object->start_coord_y);
        float curr_y = abs(object->curr_coord_y - object->start_coord_y);

        if (curr_x >= start_x && curr_y >= start_y)
        {
            start_x = object->start_coord_x;
            object->start_coord_x = object->dest_coord_x;
            object->dest_coord_x = start_x;

            start_y = object->start_coord_y;
            object->start_coord_y = object->dest_coord_y;
            object->dest_coord_y = start_y;
        }
        sfClock_Reset(object->clock_mouvement);

        sprite_SetPosition(object->sprite, (int)object->curr_coord_x, (int)object->curr_coord_y);
    }

    sprite_Draw(Game, object->sprite);

}

void object_SetPosition(Object* obj, float x, float y)
{
    obj->curr_coord_x = x;
    obj->curr_coord_y = y;

    sprite_SetPosition(obj->sprite, obj->curr_coord_x, obj->curr_coord_y);

}
