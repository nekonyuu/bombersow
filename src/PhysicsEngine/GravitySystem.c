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

#include <math.h>
#include "BaseSystem/Logging.h"
#include "Objects/GameObjects.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "PhysicsEngine/GravitySystem.h"
#include "PhysicsEngine/CollisionSystem.h"
#include "GraphicEngine/Draw.h"


void gravitysystem_PlayerUpdate(Map* map_)
{
    Config* config = map_->cfg;
    Player* player = NULL;

    float speed_y = 0, y = 0;

    Map_ClockTick(map_, PLAYER_CLOCK);

    for (unsigned int i = 0; i < map_->nb_players; i++)
    {
        player = map_->players_list[i];

        speed_y = player->speed_y + config->gravity_speed * map_->clocks_time[PLAYER_CLOCK] * config->gravity_coef;
        y = speed_y * map_->clocks_time[PLAYER_CLOCK];

        if (player->sprite->hauteur + player->coord_y + y <= config->height && player->coord_y + y > 0)
        {
            player_SetPosition(player, player->coord_x, player->coord_y + y);
            quadtree_Update(player, PLAYER);
            Collision* collision = collision_Detection_Object(player, PLAYER);
            if (collision != NULL)
            {
                player_SetPosition(player, player->coord_x, player->coord_y - y);
                quadtree_Update(player, PLAYER);
                speed_y = 0;
                player->jump = NO_JUMP;
            }
            collision_Destroy(collision);
            player->speed_y = speed_y;
        }
        else if (player->sprite->hauteur + player->coord_y + y > config->height)
        {
            player_SetPosition(player, player->coord_x, config->height - player->sprite->hauteur);
            quadtree_Update(player, PLAYER);
            player->speed_y = 0;
            player->jump = NO_JUMP;
        }
        else
            player->speed_y = speed_y;
    }
}

/*
void gravitysystem_PlayerUpdate(void* UserData)
{
    Map* map_ = (Map*) UserData;
    Config* config = map_->cfg;
    Player* player = NULL;

    float speed_y = 0, y = 0;

    while (map_->game_started)
    {
        Map_ClockTick(map_, PLAYER_CLOCK);

        for (int i = 0; i < map_->nb_players; i++)
        {
            player = map_->players_list[i];

            speed_y = player->speed_y + config->gravity_speed * map_->clocks_time[PLAYER_CLOCK] * config->gravity_coef;
            y = speed_y * map_->clocks_time[PLAYER_CLOCK];

            if (player->sprite->hauteur + player->coord_y + y <= config->height && player->coord_y + y > 0)
            {
                player_SetPosition(player, player->coord_x, player->coord_y + y);
                quadtree_Update(player, PLAYER);
                Collision* collision = collision_Detection_Object(player, PLAYER);
                if (collision != NULL)
                {
                    player_SetPosition(player, player->coord_x, player->coord_y - y);
                    quadtree_Update(player, PLAYER);
                    speed_y = 0;
                    player->jump = NO_JUMP;
                }
                collision_Destroy(collision);
                player->speed_y = speed_y;
            }
            else if (player->sprite->hauteur + player->coord_y + y > config->height)
            {
                player_SetPosition(player, player->coord_x, config->height - player->sprite->hauteur);
                quadtree_Update(player, PLAYER);
                player->speed_y = 0;
                player->jump = NO_JUMP;
            }
            else
                player->speed_y = speed_y;
        }
    }
}
*/

void gravitysystem_BulletUpdate(Map* map_)
{
    Config* config = map_->cfg;
    Bullet* bullet_ = NULL, *bullet_next = NULL;

    bullet_ = BulletList_GetHead(map_->bullets);
    bullet_next = NULL;

    Map_ClockTick(map_, BULLET_CLOCK);

    while (bullet_ != NULL)
    {
        bullet_next = bullet_GetNext(bullet_);

        float speed_x = bullet_->speed_x * map_->clocks_time[BULLET_CLOCK];
        float speed_y = bullet_->speed_y * map_->clocks_time[BULLET_CLOCK];
        unsigned int traj = (unsigned int) sqrt(speed_y * speed_y + speed_x * speed_x);
        if (bullet_->coord_x + speed_x > 0 && bullet_->coord_x + speed_x < config->width &&
                bullet_->coord_y + speed_y > 0 && bullet_->coord_y + speed_y < config->height &&
                bullet_->range > 0)
        {
            bullet_SetPosition(bullet_, bullet_->coord_x + speed_x, bullet_->coord_y + speed_y);
            bullet_->range = (traj > bullet_->range) ? 0 : bullet_->range - traj;
            quadtree_Update(bullet_, BULLET);
            Collision* collision = collision_Detection_Object(bullet_, BULLET);
            if (collision != NULL)
            {
                if (collision->type == PLAYER)
                {
                    player_BulletCollision(collision->player, bullet_, map_);
                }
                map_DelBullet(map_, bullet_);
            }
            collision_Destroy(collision);
        }
        else
        {
            map_DelBullet(map_, bullet_);
        }

        bullet_ = bullet_next;
    }
}

/*
void gravitysystem_BulletUpdate(void* UserData)
{
    Map* map_ = (Map*) UserData;
    Config* config = map_->cfg;
    Bullet* bullet_ = NULL, *bullet_next = NULL;

    while (map_->game_started)
    {
        bullet_ = BulletList_GetHead(map_->bullets);
        bullet_next = NULL;

        Map_ClockTick(map_, BULLET_CLOCK);

        while (bullet_ != NULL)
        {
            bullet_next = bullet_GetNext(bullet_);

            float speed_x = bullet_->speed_x * map_->clocks_time[BULLET_CLOCK];
            float speed_y = bullet_->speed_y * map_->clocks_time[BULLET_CLOCK];
            int traj = (int) sqrt(speed_y * speed_y + speed_x * speed_x);
            if (bullet_->coord_x + speed_x > 0 && bullet_->coord_x + speed_x < config->width &&
                    bullet_->coord_y + speed_y > 0 && bullet_->coord_y + speed_y < config->height &&
                    bullet_->range > 0)
            {
                bullet_SetPosition(bullet_, bullet_->coord_x + speed_x, bullet_->coord_y + speed_y);
                bullet_->range = (traj > bullet_->range) ? 0 : bullet_->range - traj;
                quadtree_Update(bullet_, BULLET);
                Collision* collision = collision_Detection_Object(bullet_, BULLET);
                if (collision != NULL)
                {
                    if (collision->type == PLAYER)
                    {
                        player_BulletCollision(collision->player, bullet_, map_);
                    }
                    map_DelBullet(map_, bullet_);
                }
                collision_Destroy(collision);
            }
            else
            {
                map_DelBullet(map_, bullet_);
            }

            bullet_ = bullet_next;
        }
    }
}
*/

/*void gravitysystem_BloodUpdate(Map* map_, Particle* particle_, Config* config)
{
    float speed_y = particle_->speed_y + config->gravity_speed * map_->clock_time * config->gravity_coef;
    float y = speed_y * map_->clock_time;

    float largeur;
    float hauteur;
    sfShape_GetPointPosition(particle_->shape, 2, &largeur, &hauteur);
    if (sfShape_GetY(particle_->shape) + hauteur + y <= config->height && sfShape_GetY(particle_->shape)+ y > 0)
    {
        particle_SetPosition(particle_, sfShape_GetX(particle_->shape), sfShape_GetY(particle_->shape) + y);
        particle_->speed_y = speed_y;
    }
    else if (sfShape_GetY(particle_->shape)+hauteur+y > config->height)
    {
        particle_->speed_y = 0;
    }
    else
        particle_->speed_y = 0;
}*/

void gravitysystem_BloodUpdate(void* UserData)
{
    Map* map_ = (Map*) UserData;
    Particle* particle_ = NULL;
    Config* config = map_->cfg;
    float speed_y = 0, y = 0, part_size_y = 0, part_y = 0;

    while (map_->game_started)
    {
        Map_ClockTick(map_, PARTICLE_CLOCK);
        for (int i = 0; i < map_->particle_table->nbr_particle; i++)
        {
            particle_ = map_->particle_table->particle[i];

            part_size_y = Particle_GetSizeY(particle_);
            part_y = Particle_GetY(particle_);

            speed_y = Particle_GetSpeedY(particle_) + config->gravity_speed * map_->clocks_time[PARTICLE_CLOCK] * config->gravity_coef;
            y = speed_y * map_->clocks_time[PARTICLE_CLOCK];

            if (part_y + part_size_y + y <= config->height && part_y + y > 0)
            {
                Particle_SetY(particle_, part_y + y);
                Particle_SetSpeedY(particle_, speed_y);
            }
            else
                Particle_SetSpeedY(particle_, 0);
        }
        sfSleep(0.005f);
    }
}

void gravitysystem_WorldUpdate(Map* map_)
{
    // Mise à jour Players
    gravitysystem_PlayerUpdate(map_);

    // Mise à jour Bullets
    gravitysystem_BulletUpdate(map_);
}
