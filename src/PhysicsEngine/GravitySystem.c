#include "BaseSystem/Logging.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "PhysicsEngine/GravitySystem.h"
#include "PhysicsEngine/CollisionSystem.h"
#include "Objects/GameObjects.h"
#include "GraphicEngine/Draw.h"

void gravitysystem_PlayerUpdate(Map* map_, float gravity, Player* player)
{
    float speed_y = player->speed_y+0.25*gravity*sfClock_GetTime(map_->clock);
    if(player->sprite->hauteur+player->coord_y+speed_y <= SCREEN_HEIGHT && player->coord_y+speed_y >= 0)
    {
        player_SetPosition(player, player->coord_x, player->coord_y+player->speed_y);
        Collision* collision = collision_Detection_Object(player, PLAYER);
        if(collision != NULL)
        {
            player_SetPosition(player, player->coord_x, player->coord_y-player->speed_y);
            speed_y = 0;
        }
        collision_Destroy(collision);
        player->speed_y = speed_y;
    }
    else if(player->sprite->hauteur+player->coord_y+speed_y > SCREEN_HEIGHT)
    {
        player_SetPosition(player, player->coord_x, SCREEN_HEIGHT-player->sprite->hauteur);
        player->speed_y = 0;
    }
    else
    {
        player->speed_y = speed_y;
    }


    quad_tree_Update(player, PLAYER);
}

void gravitysystem_WorldUpdate(Map* map_, float gravity)
{
    for (int i = 0; i < map_->nb_players; i++)
    {
        gravitysystem_PlayerUpdate(map_, gravity, map_->players_list[i]);
    }

    for (int i = 0; i < map_->nb_objects; i++)
    {

    }

    sfClock_Reset(map_->clock);
}
