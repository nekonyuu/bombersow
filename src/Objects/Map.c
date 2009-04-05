#include "BaseSystem/Config.h"
#include "BaseSystem/Logging.h"
#include "Objects/GameObjects.h"


// Constructeur
Map* map_Create(unsigned int map_id, unsigned int nb_players)
{
    Map* new_map = NULL;
    assert(new_map = (Map*) malloc(sizeof(Map)));

    new_map->mapId = map_id;
    new_map->background = NULL;

    new_map->images = NULL;
    new_map->animations = NULL;
    new_map->nb_anim = 0;

    new_map->objects_list = NULL;
    new_map->nb_objects = 0;

    new_map->players_list = (Player**) malloc(nb_players * sizeof(Player*));
    for (int i = 0; i < nb_players; i++)
        new_map->players_list[i] = NULL;

    new_map->nb_players = 0;

    new_map->bullets_list = NULL;
    new_map->nb_bullets = 0;

    new_map->game_packets2send = NULL;

    new_map->chat_started = false;
    new_map->game_started = false;

    new_map->game_socket = NULL;
    new_map->game_port = 0;

    return new_map;
}

// Destructeur
void map_Destroy(Map* map2destroy)
{
    if (!map2destroy)
        logging_Warning("map_Destroy", "Map object sent NULL");
    else
    {
        sfSprite_Destroy(map2destroy->background);
        for (int i = 0; i < map2destroy->nb_objects; i++)
            object_Destroy(map2destroy->objects_list[i]);

        free(map2destroy);
    }
}

void map_AddObject(Map* map_, Object* object_)
{
    if (!map_->objects_list)
        map_->objects_list = (Object**) malloc(++map_->nb_objects * sizeof(Object*));
    else
        map_->objects_list = (Object**) realloc(map_->objects_list, ++map_->nb_objects * sizeof(Object*));

    if (!map_->objects_list && !map_->objects_list[map_->nb_objects - 1])
        logging_Error("map_AddObject", "Memory allocation error");

    map_->objects_list[map_->nb_bullets - 1] = object_;
}

void map_DelObject(Map* map_, unsigned int object_id)
{
    if (!map_->objects_list[object_id])
    {
        logging_Warning("map_DelObject", "Object object at object_id is NULL");
        return;
    }

    object_Destroy(map_->objects_list[object_id]);

    for (int i = object_id; i < map_->nb_objects - 1; i++)
        map_->objects_list[i] = map_->objects_list[i + 1];

    assert(map_->objects_list = (Object**) realloc(map_->objects_list, --map_->nb_objects * sizeof(Object*)));
}

void map_AddPlayer(Map* map_, Player* player_)
{
    if (!map_->players_list)
        map_->players_list = (Player**) malloc(++map_->nb_players * sizeof(Player*));
    else
        map_->players_list = (Player**) realloc(map_->players_list, ++map_->nb_players * sizeof(Player*));

    if (!map_->players_list && !map_->players_list[map_->nb_players - 1])
        logging_Error("map_AddPlayer", "Memory allocation error");

    map_->players_list[map_->nb_players - 1] = player_;
    map_->players_list[map_->nb_players - 1]->player_id = map_->nb_players;
}

// A voir suivant la gestion des player/signaux envoyés lors de la déco
void map_DelPlayer(Map* map_, unsigned int player_id)
{
    if (!map_->players_list[player_id])
    {
        logging_Warning("map_DelPlayer", "Player object at player_id is NULL");
        return;
    }

    player_Destroy(map_->players_list[player_id]);

    for (int i = player_id; i < map_->nb_players - 1; i++)
    {
        map_->players_list[i] = map_->players_list[i + 1];
        map_->players_list[i]->player_id = i + 1;
    }

    assert(map_->players_list = (Player**) realloc(map_->players_list, --map_->nb_players * sizeof(Player*)));
}

void map_AddBullet(Map* map_, Bullet* bullet_)
{
    if (!map_->bullets_list)
        assert(map_->bullets_list = (Bullet**) malloc(++map_->nb_bullets * sizeof(Bullet*)));
    else
        assert(map_->bullets_list = (Bullet**) realloc(map_->bullets_list, ++map_->nb_bullets * sizeof(Bullet*)));

    if (!map_->bullets_list && !map_->bullets_list[map_->nb_bullets - 1])
        logging_Error("map_AddBullet", "Memory allocation error");

    map_->bullets_list[map_->nb_bullets - 1] = bullet_;
}

void map_DelBullet(Map* map_, unsigned int bullet_id)
{
    if (!map_->bullets_list[bullet_id])
    {
        logging_Warning("map_DelBullet", "Bullet object at bullet_id is NULL");
        return;
    }

    bullet_Destroy(map_->bullets_list[bullet_id]);

    for (int i = bullet_id; i < map_->nb_bullets - 1; i++)
        map_->bullets_list[i] = map_->bullets_list[i + 1];

    assert(map_->bullets_list = (Bullet**) realloc(map_->bullets_list, --map_->nb_bullets * sizeof(Bullet*)));
}

void map_UpdateDisconnectedPlayers(void* UserData)
{
    Map* map = (Map*) UserData;

    while (map->chat_started)
    {
        for (int i = 0; i < map->nb_players; i++)
            if (!map->players_list[i]->connected)
                map_DelPlayer(map, i);

        sfSleep(1.0f);
    }
}
