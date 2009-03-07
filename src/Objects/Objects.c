#include <assert.h>

#include "Objects.h"

Object* object_Create()
{
    Object* new_object = NULL;
    assert(new_object = (Object*) malloc(sizeof(Object)));

    return EXIT_SUCCESS;
}

void object_Destroy(Object* object2destroy)
{
    sfImage_Destroy(object2destroy->draw_img);
    free(object2destroy);
}
