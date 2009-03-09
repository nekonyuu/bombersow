#include <assert.h>
#include "Objects/Objects.h"

Object* object_Create()
{
    Object* new_object = NULL;
    assert(new_object = (Object*) malloc(sizeof(Object)));


    new_object->type = 0;

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

    return new_object;
}

void object_Destroy(Object* object2destroy)
{
    assert(object2destroy != NULL);
    sfImage_Destroy(object2destroy->draw_img);
    free(object2destroy);
}
