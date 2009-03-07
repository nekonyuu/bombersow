#include "Map.h"

// Constructeur
Map* init_Map()
{
    Map* new_map;

    new_map->background = NULL;
    new_map->objects = NULL;

    return new_map;
}
