/*
    GPL v3 Licence :
    Bombersow is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bombersow is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bombersow.  If not, see <http://www.gnu.org/licenses/>.


    Creative Commons BY-NC-SA :
    This work is licensed under the Creative Commons Attribution-Noncommercial-Share Alike 3.0 Unported License.
    To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter
    to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.

*/

#include <string.h>
#include "BaseSystem/Config.h"
#include "BaseSystem/Logging.h"
#include "Objects/GameObjects.h"
#include "Networking/Networking.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "PhysicsEngine/ParticleSystem.h"
#include "GraphicEngine/Draw.h"

// Constructeur
Map* map_Create(unsigned int map_id, unsigned int nb_players, Config* config)
{
    logging_Info("map_Create", "Base creation of the map...");
    Map* new_map = NULL;
    assert(new_map = (Map*) malloc(sizeof(Map)));

    new_map->mapId = map_id;
    new_map->max_players = nb_players;
    new_map->cpt_players_rev = 0;

    new_map->cfg = config;
    new_map->particle_engine = NULL;

    new_map->background = NULL;

    new_map->images = NULL;
    new_map->animations = NULL;
    new_map->nb_anim = 0;

    new_map->objects_list = NULL;
    new_map->nb_objects = 0;

    if (nb_players > 0)
    {
        new_map->players_list = (Player**) malloc(nb_players * sizeof(Player*));
        for (unsigned int i = 0; i < nb_players; i++)
            new_map->players_list[i] = NULL;
    }
    else // Ce cas ne doit pas arriver en pratique, implémenté pour tests
        new_map->players_list = NULL;

    new_map->nb_players = 0;

    new_map->bullets = BulletList_Create();

    new_map->gamepackets2send = NULL;

    new_map->chat_started = false;
    new_map->game_started = false;

    new_map->tcp_selector = sfSelectorTCP_Create();
    new_map->game_socket = sfSocketUDP_Create();
    new_map->game_port = 0;

    for (int i = 0; i < NB_CLOCKS; i++)
    {
        new_map->clocks[i] = sfClock_Create();
        new_map->clocks_time[i] = 0;
    }

    new_map->quad_tree = quadtree_Create();
    new_map->quad_tree->rect.Left = 0;
    new_map->quad_tree->rect.Top = 0;
    new_map->quad_tree->rect.Bottom = config->height;
    new_map->quad_tree->rect.Right = config->width;

    new_map->quad_tree->first = new_map->quad_tree;

    logging_Info("map_Create", "Map created !");

    return new_map;
}

// Destructeur
void map_Destroy(Map* map2destroy)
{
    if (!map2destroy)
        logging_Warning("map_Destroy", "Map object sent NULL");
    else
    {
        logging_Info("map_Destroy", "Destroy sent map...");
        logging_Info("map_Destroy", "Destroy background...");
        sfSprite_Destroy(map2destroy->background);

        logging_Info("map_Destroy", "Destroy animations...");
        for (unsigned int i = 0; i < map2destroy->nb_anim; i++)
            animation_Destroy(map2destroy->animations[i]);

        logging_Info("map_Destroy", "Destroy images...");
        image_Destroy(map2destroy->images);

        logging_Info("map_Destroy", "Destroy objects...");
        for (unsigned int i = 0; i < map2destroy->nb_objects; i++)
            object_Destroy(map2destroy->objects_list[i]);
        free(map2destroy->objects_list);

        logging_Info("map_Destroy", "Destroy players...");
        for (unsigned int i = 0; i < map2destroy->nb_players; i++)
            player_Destroy(map2destroy->players_list[i]);
        free(map2destroy->players_list);

        logging_Info("map_Destroy", "Destroy bullets...");
        BulletList_Destroy(map2destroy->bullets);

        logging_Info("map_Destroy", "Destroy pending packets...");
        if (map2destroy->gamepackets2send)
            for (unsigned int i = 0; i < map2destroy->gamepackets2send->nb_packets; i++)
                packet_Destroy(map2destroy->gamepackets2send->packets[i]);

        logging_Info("map_Destroy", "Destroy network sockets...");

        sfSocketUDP_Destroy(map2destroy->game_socket);
        sfSelectorTCP_Destroy(map2destroy->tcp_selector);

        logging_Info("map_Destroy", "Destroy clocks...");
        for (int i = 0; i < NB_CLOCKS; i++)
            sfClock_Destroy(map2destroy->clocks[i]);

        logging_Info("map_Destroy", "Destroy quad tree...");
        quadtree_Destroy(map2destroy->quad_tree);

        free(map2destroy);
        logging_Info("map_Destroy", "Map destroyed !");
    }
}

void map_AddObject(Map* map_, Object* object_)
{
    if (!map_->objects_list)
        map_->objects_list = (Object**) malloc(++map_->nb_objects * sizeof(Object*));
    else
        map_->objects_list = (Object**) realloc(map_->objects_list, ++map_->nb_objects * sizeof(Object*));

    if (!map_->objects_list && !map_->objects_list[map_->nb_objects - 1])
        logging_Error("map_AddObject", "Memory allocation error", LOW_MEMORY_ERROR);

    map_->objects_list[map_->nb_objects - 1] = object_;

    quadtree_Add(map_->quad_tree, object_, OBJECT);
}

void map_DelObject(Map* map_, unsigned int object_id)
{
    if (!map_->objects_list[object_id])
    {
        logging_Warning("map_DelObject", "Object object at object_id is NULL");
        return;
    }

    object_Destroy(map_->objects_list[object_id]);

    for ( unsigned int i = object_id; i < map_->nb_objects - 1; i++)
        map_->objects_list[i] = map_->objects_list[i + 1];

    quadtree_Delete_Elt(map_->objects_list[object_id], OBJECT);

    assert(map_->objects_list = (Object**) realloc(map_->objects_list, --map_->nb_objects * sizeof(Object*)));

}

void map_AddPlayer(Map* map_, Player* player_)
{
    if (!map_ && !player_)
    {
        logging_Warning("map_AddPlayer", "Map or Player object sent NULL");
    }
    if (map_->max_players > map_->nb_players)
    {
        logging_Info("map_AddPlayer", "Adding player...");
        map_->nb_players++;
        map_->players_list[map_->nb_players - 1] = player_;
        if (map_->players_list[map_->nb_players - 1]->player_id == 0)
            map_->players_list[map_->nb_players - 1]->player_id = ++map_->cpt_players_rev;

        quadtree_Add(map_->quad_tree, player_, PLAYER);
        logging_Info("map_AddPlayer", "Player added !");
    }
}

void map_DelPlayer(Map* map_, unsigned int player_id)
{
    Player* ptr = map_GetPlayerFromID(map_, player_id);

    if (!ptr)
    {
        logging_Warning("map_DelPlayer", "Player object at player_id is NULL");
        return;
    }

    logging_Info("map_DelPlayer", "Remove player from QuadTree...");
    quadtree_Delete_Elt(ptr, PLAYER);

    logging_Info("map_DelPlayer", "Destroy player...");
    player_Destroy(ptr);

    logging_Info("map_DelPlayer", "Fill vacant case...");
    for (unsigned int i = player_id; i < map_->nb_players - 1; i++)
        map_->players_list[i] = map_->players_list[i + 1];

    map_->players_list[map_->nb_players - 1] = NULL;
    map_->nb_players--;
    logging_Info("map_DelPlayer", "Player deleted from map !");
}

void map_AddBullet(Map* map_, Bullet* bullet_)
{
    logging_Info("map_AddBullet", "Adding bullet to map...");

    if (!bullet_)
    {
        logging_Warning("map_AddBullet", "Bullet object sent NULL");
        return;
    }

    logging_Info("map_AddBullet", "Adding bullet to list...");
    BulletList_AddBullet(map_->bullets, bullet_);

    logging_Info("map_AddBullet", "Adding bullet to quadtree...");
    quadtree_Add(map_->quad_tree, bullet_, BULLET);
}

void map_DelBullet(Map* map_, Bullet* bullet)
{
    if (!bullet)
    {
        logging_Warning("map_DelBullet", "Bullet object sent NULL");
        return;
    }
    logging_Info("map_DelBullet", "Deleting sent bullet...");

    quadtree_Delete_Elt(bullet, BULLET);
    BulletList_DeleteBullet(map_->bullets, bullet);
}

void map_UpdateDisconnectedPlayers(void* UserData)
{
    Map* map = (Map*) UserData;

    while (map->chat_started)
    {
        for (unsigned int i = 0; i < map->nb_players; i++)
            if (!map->players_list[i]->connected)
            {
                logging_Info("map_UpdateDisconnectedPlayers", "Destroying flagged player...");
                map_DelPlayer(map, map->players_list[i]->player_id);
            }

        sfSleep(1.0f);
    }
}

// Retourne le player ayant l'id player_id
Player* map_GetPlayerFromID(Map* map, unsigned int player_id)
{
    Player* ptr = NULL;

    for (unsigned int i = 0; i < map->nb_players; i++)
    {
        if (player_id == map->players_list[i]->player_id)
        {
            ptr = map->players_list[i];
            break;
        }
    }

    return ptr;
}

// Retourne l'id du player ayant le nom name
unsigned int map_GetPlayerIDFromName(Map* map, char* name)
{
    for (unsigned int i = 0; i < map->nb_players; i++)
    {
        if (!strcmp(name, map->players_list[i]->char_name))
            return map->players_list[i]->player_id;
    }

    return 0;
}

void map_SetGamePort(Map* map, unsigned int port)
{
    map->game_port = port;
}

void map_SetCptCurrPlayers(Map* map, unsigned int cpt)
{
    map->cpt_players_rev = cpt;
}

void Map_ClockTick(Map* map_, ClockType type)
{
    float time = 0;

    map_->clocks_time[type] = ((time = sfClock_GetTime(map_->clocks[type])) > 0) ? time : 0;
    sfClock_Reset(map_->clocks[type]);
}

// Dessin de la map
void map_Draw(sfRenderWindow* Game, Map* map)
{
    for (unsigned int i = 0; i < map->nb_objects; i++)
        object_Draw(Game, map->objects_list[i]);

    for (unsigned int i = 0; i < map->nb_players; i++)
        player_Draw(Game, map->players_list[i]);

    for (unsigned int i = 0; i < map->nb_players; i++)
        player_DrawLife(Game, map->players_list[i]);

    bullet_DrawList(Game, BulletList_GetHead(map->bullets));

    particle_table_Draw(Game, map->particle_engine->table, map->cfg);
}
