#include <assert.h>
#include "Objects/Objects.h"

Object* object_Create()
{
    Object* new_object = NULL;
    assert(new_object = (Object*) malloc(sizeof(Object)));

    // TODO : Chargement du sprite et ses coordonnées

    return new_object;
}

void object_Destroy(Object* object2destroy)
{
    assert(object2destroy != NULL);
    sfSprite_Destroy(object2destroy->draw_img);
    free(object2destroy);
}
