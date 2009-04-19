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

    new_player->sprite = NULL;

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

// Déplacement du personnage sur la map
void player_Displace(Player* player_, float x, float y)
{
    float temp_x = player_->coord_x + x;
    float temp_y = player_->coord_y + y;

    if (temp_x <= SCREEN_WIDTH)
        if (temp_x < 0)
            player_->coord_x = 0;
        else
            player_->coord_x = temp_x;
    else
        player_->coord_x = SCREEN_WIDTH;

    if (temp_y <= SCREEN_WIDTH)
        if (temp_y < 0)
            player_->coord_y = 0;
        else
            player_->coord_y = temp_y;
    else
        player_->coord_y = SCREEN_WIDTH;
}

void player_SwitchWeapon(Player* player_, int weapon_type)
{
    if (weapon_type < NB_MAX_WEAPONS && player_->weapons[weapon_type]->collected)
        player_->current_weapon = weapon_type;
}

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

// Fonction qui gère le saut du joueur, TODO : Trajectoire lors du saut, vecteur force
void player_Jump(Player* player_)
{
    assert(player_);
    if (player_->jump == NO_JUMP)
    {
        player_->jump = SIMPLE_JUMP;
    }
    else if (player_->jump == SIMPLE_JUMP)
    {
        player_->jump = NO_JUMP;
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
