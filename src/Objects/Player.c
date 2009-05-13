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
#include "BaseSystem/Logging.h"
#include "GraphicEngine/Draw.h"
#include "Objects/GameObjects.h"

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

    new_player->player_thread = NULL;
    new_player->listen_socket = NULL;

    new_player->connected = sfTrue;
    new_player->ready = sfFalse;

    // TODO : Image Player
    new_player->sprite = sprite_Create(0, 0, player_default_image, NULL);

    new_player->quad_node = NULL;
    new_player->list_node = NULL;

    new_player->gravity = 0;

    return new_player;
}

// Destructeur
void player_Destroy(Player* player2destroy)
{
    if (!player2destroy)
        logging_Warning("player_Destroy", "Player object sent NULL");
    else
    {
        sfString_Destroy(player2destroy->name);
        free_secure(player2destroy->char_name);

        for (int i = 0; i < NB_MAX_WEAPONS; i++)
            weapon_Destroy(player2destroy->weapons[i]);

        sprite_Destroy(player2destroy->sprite);
        free_secure(player2destroy->weapons);
        free_secure(player2destroy);
    }
}

Player* player_GetPlayerFromID(Map* map, unsigned int player_id)
{
    int i = 0;
    for (i = 0; i < map->nb_players; i++)
        if (player_id == map->players_list[i]->player_id)
            break;

    return map->players_list[i];
}

// Déplacement du personnage sur la map sur x et y, y seulement pour le jetpack
// TODO : gestion collisions
void player_Displace(Player* player_, Direction move, float time, Config* config)
{
    float temp_x, temp_y;

    if (!player_->jetpack_mode)
    {
        if (move == UP)
        {
            player_Jump(player_, config);
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
                player_->coord_x = 0;
            else
                player_->coord_x = temp_x;
        }
        else
            player_->coord_x = config->width - player_->sprite->largeur;
    }
    else
    {
        temp_x = player_->coord_x + ((move == LEFT) ? -config->fly_speed * time : (move == RIGHT) ? config->fly_speed * time : 0);
        temp_y = player_->coord_y + ((move == UP) ? -config->fly_speed * time : (move == DOWN) ? config->fly_speed * time : 0);
        if (temp_y + player_->sprite->hauteur <= config->height)
        {
            if (temp_y < 0)
                player_->coord_y = 0;
            else
                player_->coord_y = temp_y;
        }
        else
            player_->coord_y = config->height - player_->sprite->hauteur;
    }
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

// Diminue le nombre de cartouches restantes et crée un/des bullet(s)
void player_WeaponShoot(Map* map, Player* player_)
{
    if (player_->weapons[player_->current_weapon]->type == SHOTGUN)
    {
        for (int nb_bullet = 0; nb_bullet < SHOTGUN_SHRAPNELS; nb_bullet++)
            map_AddBullet(map, bullet_Create(player_->player_id, player_->current_weapon));
    }
    else
        map_AddBullet(map, bullet_Create(player_->player_id, player_->current_weapon));

    player_->weapons[player_->current_weapon]->nb_curr_bullets--;

}

// Fonction qui gère le saut du joueur
void player_Jump(Player* player_, Config* config)
{
    assert(player_);
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
}

void player_SetPosition(Player* player, float x, float y)
{
    player->coord_x = x;
    player->coord_y = y;
    sprite_SetPosition(player->sprite, x, y);
}

void player_Draw(sfRenderWindow* Game, Player* player)
{
    sprite_Draw(Game, player->sprite);
}
