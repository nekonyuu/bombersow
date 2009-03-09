#include <assert.h>
#include "Objects/Objects.h"
#include "Objects/Map.h"

// Constructeur
Map* map_Create()
{
    Map* new_map = NULL;
    assert(new_map = (Map*) malloc(sizeof(Map)));

    new_map->background = NULL;
    new_map->objects = NULL;
    new_map->nb_objects = 0;
    new_map->nb_players = 0;

    assert(new_map->player_list = (Player**) malloc(sizeof(Player*)));
    for (int i = 0; i < NB_MAX_PLAYERS; i++)
        new_map->player_list[i] = NULL;

    return new_map;
}

// Destructeur
void map_Destroy(Map* map2destroy)
{
    assert(map2destroy != NULL);

    sfSprite_Destroy(map2destroy->background);
    for (int i = 0; i < map2destroy->nb_objects; i++)
        object_Destroy(map2destroy->objects[i]);

    free(map2destroy);
}


