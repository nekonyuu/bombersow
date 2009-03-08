#include <assert.h>

#include "Objects/Objects.h"

Object* object_Create()
{
    Object* new_object = NULL;
    assert(new_object = (Object*) malloc(sizeof(Object)));

    return new_object;
}

void object_Destroy(Object* object2destroy)
{
    sfImage_Destroy(object2destroy->draw_img);
    free(object2destroy);
}
