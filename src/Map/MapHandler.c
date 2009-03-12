#include <assert.h>
#include "Objects/Map.h"
#include "Map/MapLoader.h"
#include "Map/MapHandler.h"

// Ajout d'un joueur sur la map
int map_AddPlayer(Map* map, Player* player)
{
    assert(map != NULL && player != NULL);

    if (map->nb_players + 1 > NB_MAX_PLAYERS)
        return EXIT_FAILURE;

    map->nb_players++;
    map->player_list[map->nb_players - 1] = player;
    return EXIT_SUCCESS;
}

// Suppression d'un joueur sur la map
int map_DelPlayer(Map* map, Player* player)
{
    assert(map != NULL && player != NULL);

    if (map->nb_players - 1 < 0)
        return EXIT_FAILURE;

    map->nb_players--;
    map->player_list[map->nb_players] = NULL;
    return EXIT_SUCCESS;
}
