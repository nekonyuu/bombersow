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
#include <string.h>
#include <time.h>

#include "BaseSystem/Config.h"
#include "BaseSystem/Logging.h"
#include "GraphicEngine/Draw.h"
#include "PhysicsEngine/CollisionSystem.h"

sfShape* life_Create(Player* player)
{
    sfShape* life = NULL;
    if (player->life < 50)
        life = sfShape_CreateRectangle(0, 0, player->life/2, 3, sfColor_FromRGB(255,0,0), 0, sfColor_FromRGB(255,255,255));
    else
        life = sfShape_CreateRectangle(0, 0, player->life/2, 3, sfColor_FromRGB(0,255,0), 0, sfColor_FromRGB(255,255,255));
    life_SetPosition(life, player);
    return life;
}

void life_SetPosition(sfShape* life, Player* player)
{
    float y = player->coord_y + player->sprite->hauteur;
    float x = player->sprite->largeur/2 + player->coord_x - player->life/4;
    sfShape_SetPosition(life, x, y);
}

// Constructeur
Player* player_Create(char* name, unsigned int current_weapon)
{
    Player* new_player = NULL;

    assert(new_player = (Player*) malloc(sizeof(Player)));

    new_player->name = sfString_Create();
    sfString_SetText(new_player->name, name);

    new_player->char_name = name;

    new_player->player_id = 0;
    new_player->life = 100;

    assert(new_player->weapons = (Weapon **) malloc(NB_MAX_WEAPONS * sizeof(Weapon*)));
    for (int i = 0; i < NB_MAX_WEAPONS; i++)
        new_player->weapons[i] = weapon_Create(i);

    new_player->current_weapon = current_weapon;                // Arme choisie dès le spawn
    new_player->weapons[current_weapon]->collected = true;      // Arme active
    new_player->nb_weapons = 1;

    new_player->weapon_reload = sfClock_Create();

    /* TODO : Fonction donnant les coordonnées de spawn
    new_player->coord_x = ;                                     // Coordonnées de spawn
    new_player->coord_y = ;
    */

    new_player->m_coord_x = 0;
    new_player->m_coord_y = 0;

    new_player->speed_x = 0;
    new_player->speed_y = 0;

    new_player->jump = NO_JUMP;
    new_player->jetpack_mode = false;

    new_player->frags = 0;
    new_player->killed = 0;

    new_player->listen_socket = NULL;

    new_player->connected = sfTrue;
    new_player->ready = sfFalse;

    // TODO : Image Player
    new_player->sprite = sprite_Create(0, 0, player_default_image, NULL);

    new_player->quad_node = NULL;
    new_player->list_node = NULL;

    new_player->gravity = 0;

    new_player->shape_life = life_Create(new_player);
    new_player->clock_respawn = sfClock_Create();
    new_player->player_dead = 0;

    return new_player;
}

// Destructeur
void player_Destroy(Player* player2destroy)
{
    if (!player2destroy)
        logging_Warning("player_Destroy", "Player object sent NULL");
    else
    {
        logging_Info("player_Destroy", "Destroy sent player...");
        logging_Info("player_Destroy", "Destroy sfString...");
        sfString_Destroy(player2destroy->name);
        logging_Info("player_Destroy", "Destroy name...");
        free(player2destroy->char_name);

        logging_Info("player_Destroy", "Destroy weapons...");
        for (int i = 0; i < NB_MAX_WEAPONS; i++)
            weapon_Destroy(player2destroy->weapons[i]);
        free(player2destroy->weapons);

        sfShape_Destroy(player2destroy->shape_life);
        sfClock_Destroy(player2destroy->clock_respawn);

        logging_Info("player_Destroy", "Destroy player's sprite...");
        sprite_Destroy(player2destroy->sprite);
        logging_Info("player_Destroy", "Destroy player's TCP socket...");
        sfSocketTCP_Destroy(player2destroy->listen_socket);
        free(player2destroy);
        logging_Info("player_Destroy", "Player destroyed !");
    }
}

// Déplacement du personnage sur la map sur x et y, y seulement pour le jetpack
void player_Displace(Player* player_, Direction move, float time, Config* config)
{
    float temp_x = 0, temp_y = 0, old_x = player_->coord_x, old_y = player_->coord_y, final_x = 0, final_y = 0;

    if (!player_->jetpack_mode)
    {
        if (move == UP)
        {
            if (player_->jump == NO_JUMP)
            {
                player_->speed_y = config->jump_speed * config->gravity_coef;
                player_->jump = SIMPLE_JUMP;
            }
            else if (player_->jump == SIMPLE_JUMP)
            {
                player_->speed_y = config->jump_speed * config->gravity_coef;
                player_->jump = DOUBLE_JUMP;
            }
            return;
        }

        if (move == DOWN && !(player_->jump == FALLING))
        {
            player_->speed_y += config->force_fall_speed;
            player_->jump = FALLING;
            return;
        }

        temp_x = player_->coord_x + ((move == LEFT) ? -config->move_speed * time : (move == RIGHT) ? config->move_speed * time : 0);
        if (temp_x + player_->sprite->largeur <= config->width)
        {
            if (temp_x < 0)
                final_x = 0;
            else
                final_x = temp_x;
        }
        else
            final_x = config->width - player_->sprite->largeur;

        final_y = player_->coord_y;
    }
    else
    {
        temp_x = player_->coord_x + ((move == LEFT) ? -config->fly_speed * time : (move == RIGHT) ? config->fly_speed * time : 0);
        temp_y = player_->coord_y + ((move == UP) ? -config->fly_speed * time : (move == DOWN) ? config->fly_speed * time : 0);
        if (temp_y + player_->sprite->hauteur <= config->height)
        {
            if (temp_y < 0)
                final_y = 0;
            else
                final_y = temp_y;
        }
        else
            final_y = config->height - player_->sprite->hauteur;
    }

    player_SetPosition(player_, final_x, final_y);
    quadtree_Update(player_, PLAYER);
    Collision* collision = collision_Detection_Object(player_, PLAYER);
    if (collision != NULL)
    {
        player_SetPosition(player_, old_x, old_y);
        quadtree_Update(player_, PLAYER);
    }
    collision_Destroy(collision);

    return;
}

// Changement d'arme
void player_SwitchWeapon(Player* player_, int weapon_type)
{
    if (weapon_type < NB_MAX_WEAPONS && player_->weapons[weapon_type]->collected)
        player_->current_weapon = weapon_type;
}

// Récupération d'arme
void player_CollectWeapon(Player* player_, int weapon_type)
{
    if (weapon_type < NB_MAX_WEAPONS)
        player_->weapons[weapon_type]->collected = true;
}

void player_Damage(Player* player_, int damage)
{
    player_->life = ((int)player_->life-damage > 0) ? player_->life-damage : 0;

    sfShape_Destroy(player_->shape_life);
    player_->shape_life = life_Create(player_);

    if (player_->life == 0)
    {
        player_->player_dead = 1;
        quadtree_Delete_Elt(player_, PLAYER);
        sfClock_GetTime(player_->clock_respawn);
    }
}

// Diminue le nombre de cartouches restantes et crée un/des bullet(s)
void player_WeaponShoot(Map* map, Player* player_, float mouse_x, float mouse_y)
{
    float x_center = player_->coord_x + player_->sprite->largeur / 2;
    float y_center = player_->coord_y + player_->sprite->hauteur / 2;

    float diagonale = sqrt((mouse_x - x_center) * (mouse_x - x_center) + (mouse_y - y_center) * (mouse_y - y_center));

    float vec_x = (mouse_x - x_center) / diagonale;
    float vec_y = (mouse_y - y_center) / diagonale;

    if (sfClock_GetTime(player_->weapon_reload) >= player_->weapons[player_->current_weapon]->reload_latency)
    {
        sfClock_Reset(player_->weapon_reload);

        float proj_speed = player_->weapons[player_->current_weapon]->proj_speed;

        if (player_->weapons[player_->current_weapon]->type == SHOTGUN)
        {
            for (int nb_bullet = 0; nb_bullet < SHOTGUN_SHRAPNELS; nb_bullet++)
            {
                Bullet* bullet = bullet_Create(player_->player_id, player_->current_weapon);

                vec_x = (mouse_x - x_center) / diagonale;
                vec_y = (mouse_y - y_center) / diagonale;

                bullet_SetPosition(bullet, x_center + vec_x * 30, y_center + vec_y * 30);

                vec_x = proj_speed * (mouse_x - x_center) / diagonale;
                vec_y = proj_speed * (mouse_y - y_center) / diagonale;

                float angle = (nb_bullet % 2) ? -(nb_bullet / 2 + 1) * 3.14 / 36 : (nb_bullet / 2)* 3.14 / 36;
                float vec_x2 = vec_x * cos(angle) + vec_y * sin(angle);
                float vec_y2 = -vec_x * sin(angle) + vec_y * cos(angle);
                bullet_SetSpeed(bullet, vec_x2, vec_y2);

                map_AddBullet(map, bullet);
            }
        }
        else
        {
            Bullet* bullet = bullet_Create(player_->player_id, player_->current_weapon);

            bullet_SetPosition(bullet, x_center+vec_x*30, y_center+vec_y*30);

            vec_x = proj_speed * (mouse_x - x_center) / diagonale;
            vec_y = proj_speed * (mouse_y - y_center) / diagonale;

            bullet_SetSpeed(bullet, vec_x, vec_y);

            map_AddBullet(map, bullet);
        }

        player_->weapons[player_->current_weapon]->nb_curr_bullets--;
    }
}

unsigned int player_GetPlayerID(Player* player)
{
    return player->player_id;
}

void player_SetPosition(Player* player, float x, float y)
{
    player->coord_x = x;
    player->coord_y = y;
    sprite_SetPosition(player->sprite, x, y);
    life_SetPosition(player->shape_life, player);
}

void player_Draw(sfRenderWindow* Game, Player* player)
{
    if (!player->player_dead)
        sprite_Draw(Game, player->sprite);
}

void player_DrawLife(sfRenderWindow* Game, Player* player)
{
    sfRenderWindow_DrawShape(Game, player->shape_life);
}

void player_BulletCollision(Player* player, Bullet* bullet, Map* map)
{
    player_Damage(player, bullet->damage);

    ParticleEngine_CreateBlood(map->particle_engine, bullet->coord_x, bullet->coord_y, bullet->speed_x, bullet->speed_y);

    /*
    for (int i = 0; i < 30; i++)
    {
        Particle* particle = particle_CreateBlood();
        particle_SetPosition(particle, bullet->coord_x, bullet->coord_y);
        particle->speed_x = bullet->speed_x/(3+(i%5));
        particle->speed_y = bullet->speed_y/(5+(i%5));

        float angle = (i%2) ? -(i/2+1)* 3.14/72 : (i/2)* 3.14/72;
        float vec_x2 = particle->speed_x * cos(angle) + particle->speed_y * sin(angle);
        float vec_y2 = -particle->speed_x * sin(angle) + particle->speed_y * cos(angle);

        particle->speed_x = vec_x2;
        particle->speed_y = vec_y2;

        particle_table_AddParticle(map->particle_table, particle);
    }*/
    //player_CreateBlood(player, bullet->coord_x, bullet->coord_y);
}
/*
void player_CreateBlood(Player* player, float x, float y)
{

}*/
