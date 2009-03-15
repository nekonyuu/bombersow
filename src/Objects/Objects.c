#include "BaseSystem/Logging.h"
#include "Objects/GameObjects.h"

Object* object_Create()
{
    Object* new_object = NULL;
    assert(new_object = (Object*) malloc(sizeof(Object)));

    new_object->objectID = 0;
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

    new_object->spawned = true;

    stobject_Create(new_object);

    return new_object;
}

void object_Destroy(Object* object2destroy)
{
    assert(object2destroy != NULL);
    sfSprite_Destroy(object2destroy->draw_img);
    stobject_Destroy(object2destroy->stripped);
    free(object2destroy);
}

void stobject_Create(Object* object_)
{
    assert(object_);

    assert(object_->stripped = (stObject*) malloc(sizeof(stObject)));

    object_->stripped->objectID = (sfUint8) object_->objectID;
    object_->stripped->coord_x = object_->curr_coord_x;
    object_->stripped->coord_y = object_->curr_coord_y;
    object_->stripped->speed = (sfUint8) object_->speed;
}

void stobject_Destroy(stObject* stobject_)
{
    if (!stobject_)
    {
        logging_Warning("stobject_Destroy", "StObject object sent NULL");
        return;
    }

    free(stobject_);
}

void stobject_Update(Object* object_)
{
    object_->stripped->coord_x = object_->curr_coord_x;
    object_->stripped->coord_y = object_->curr_coord_y;
    object_->stripped->speed = object_->speed;
}
