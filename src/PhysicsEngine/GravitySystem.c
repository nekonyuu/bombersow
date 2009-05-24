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

void gravitysystem_PlayerUpdate(Map* map_, Player* player, Config* config)
{
    float speed_y = player->speed_y + config->gravity_speed * map_->clock_time * config->gravity_coef;
    float y = speed_y * map_->clock_time;

    if(player->sprite->hauteur + player->coord_y + y <= config->height && player->coord_y + y > 0)
    {
        player_SetPosition(player, player->coord_x, player->coord_y + y);
        quadtree_Update(player, PLAYER);
        Collision* collision = collision_Detection_Object(player, PLAYER);
        if(collision != NULL)
        {
            player_SetPosition(player, player->coord_x, player->coord_y - y);
            quadtree_Update(player, PLAYER);
            speed_y = 0;
            player->jump = NO_JUMP;
        }
        collision_Destroy(collision);
        player->speed_y = speed_y;
    }
    else if(player->sprite->hauteur + player->coord_y + y > config->height)
    {
        player_SetPosition(player, player->coord_x, config->height - player->sprite->hauteur);
        quadtree_Update(player, PLAYER);
        player->speed_y = 0;
        player->jump = NO_JUMP;
    }
    else
        player->speed_y = speed_y;
}

void gravitysystem_BulletUpdate(Map* map_, Bullet* bullet_, Config* config)
{
    float speed_x = bullet_->speed_x * map_->clock_time;
    float speed_y = bullet_->speed_y * map_->clock_time;
    bullet_SetPosition(bullet_, bullet_->coord_x+speed_x, bullet_->coord_y+speed_y);
}

void gravitysystem_WorldUpdate(Map* map_, Config* config)
{
    map_->clock_time = (sfClock_GetTime(map_->clock) > 0) ? sfClock_GetTime(map_->clock) : 0;

    for (int i = 0; i < map_->nb_players; i++)
    {
        gravitysystem_PlayerUpdate(map_,map_->players_list[i], config);
    }

    for (int i = 0; i < map_->nb_bullets; i++)
    {
        gravitysystem_BulletUpdate(map_,map_->bullets_list[i], config);
    }

    sfClock_Reset(map_->clock);
}
