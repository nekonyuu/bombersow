#include "BaseSystem/Logging.h"
#include "Objects/Objects.h"

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

    new_object->stripped = stobject_Create(new_object);

    return new_object;
}

void object_Destroy(Object* object2destroy)
{
    assert(object2destroy != NULL);
    sfSprite_Destroy(object2destroy->draw_img);
    stobject_Destroy(object2destroy->stripped);
    free(object2destroy);
}

stObject* stobject_Create(Object* object_)
{
    stObject* new_stobject;
    assert(new_stobject = (stObject*) malloc(sizeof(stObject)));

    new_stobject->objectID = &object_->objectID;

    new_stobject->start_coord_x = &object_->start_coord_x;
    new_stobject->start_coord_y = &object_->start_coord_y;

    new_stobject->dest_coord_x = &object_->dest_coord_x;
    new_stobject->dest_coord_y = &object_->dest_coord_y;

    return new_stobject;
}

void stobject_Destroy(stObject* stobject_)
{
    if (!stobject_)
    {
        logging_Warning("stobject_Destroy", "StObject object sent NULL");
        return;
    }
    stobject_->start_coord_x = NULL;
    stobject_->start_coord_y = NULL;

    stobject_->dest_coord_x = NULL;
    stobject_->dest_coord_y = NULL;

    stobject_->speed = NULL;

    free(stobject_);
}
