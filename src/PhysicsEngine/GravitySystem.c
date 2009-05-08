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

#include "BaseSystem/Logging.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "PhysicsEngine/GravitySystem.h"
#include "PhysicsEngine/CollisionSystem.h"
#include "Objects/GameObjects.h"
#include "GraphicEngine/Draw.h"

void gravitysystem_PlayerUpdate(Map* map_, float gravity, Player* player)
{

    float speed_y = player->speed_y+gravity*gravity*map_->clock_time;
    float y = speed_y * map_->clock_time;
    if(player->sprite->hauteur + player->coord_y + y <= SCREEN_HEIGHT && player->coord_y + y > 0)
    {
        player_SetPosition(player, player->coord_x, player->coord_y+y);
        quad_tree_Update(player, PLAYER);
        Collision* collision = collision_Detection_Object(player, PLAYER);
        if(collision != NULL)
        {
            player_SetPosition(player, player->coord_x, player->coord_y-y);
            quad_tree_Update(player, PLAYER);
            speed_y = 0;
        }
        collision_Destroy(collision);
        player->speed_y = speed_y;
    }
    else if(player->sprite->hauteur+player->coord_y+y > SCREEN_HEIGHT)
    {
        player_SetPosition(player, player->coord_x, SCREEN_HEIGHT-player->sprite->hauteur);
        quad_tree_Update(player, PLAYER);
        player->speed_y = 0;
    }
    else
    {
        player->speed_y = speed_y;
    }
}

void gravitysystem_WorldUpdate(Map* map_, float gravity)
{
    map_->clock_time = (sfClock_GetTime(map_->clock) > 0) ? sfClock_GetTime(map_->clock) : 0;
    for (int i = 0; i < map_->nb_players; i++)
    {
        gravitysystem_PlayerUpdate(map_, gravity, map_->players_list[i]);
    }

    for (int i = 0; i < map_->nb_objects; i++)
    {

    }

    sfClock_Reset(map_->clock);
}
