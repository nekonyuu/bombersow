#include <assert.h>

#include "Objects.h"
#include "Map.h"

// Constructeur
Map* map_Create()
{
    Map* new_map = NULL;
    assert(new_map = (Map*) malloc(sizeof(Map)));

    new_map->background = NULL;
    new_map->objects = NULL;
    new_map->nb_objects = 0;

    return new_map;
}

// Destructeur
void map_Destroy(Map* map2destroy)
{
    sfImage_Destroy(map2destroy->background);
    for(int i = 0; i < map2destroy->nb_objects; i++)
        object_Destroy(map2destroy->objects[i]);

    free(map2destroy);
}
