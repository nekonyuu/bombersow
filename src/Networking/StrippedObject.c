#include <stdio.h>
#include <assert.h>
#include "Objects/Objects.h"
#include "Networking/StrippedObject.h"

StObject* stobject_Create(Object* object_)
{
    StObject* new_stobject;
    assert(new_stobject = (StObject*) malloc(sizeof(StObject)));

    new_stobject->objectID = object_->objectID;

    new_stobject->start_coord_x = &object_->start_coord_x;
    new_stobject->start_coord_y = &object_->start_coord_y;

    new_stobject->dest_coord_x = &object_->dest_coord_x;
    new_stobject->dest_coord_y = &object_->dest_coord_y;

    return new_stobject;
}

void stobject_Destroy(StObject* stobject_)
{
    if(!stobject_)
    {
        printf("Warning - stobject_Destroy : StObject object sent NULL\n");
        return;
    }
    stobject_->start_coord_x = NULL;
    stobject_->start_coord_y = NULL;

    stobject_->dest_coord_x = NULL;
    stobject_->dest_coord_y = NULL;

    stobject_->speed = NULL;

    free(stobject_);
}
