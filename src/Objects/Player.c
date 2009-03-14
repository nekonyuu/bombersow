#include <string.h>
#include "BaseSystem/Logging.h"
#include "GraphicEngine/Draw.h"
#include "Objects/Player.h"
#include "Objects/Weapon.h"

// Constructeur
Player* player_Create(char* name, unsigned int current_weapon)
{
    Player* new_player = NULL;

    assert(new_player = (Player*) malloc(sizeof(Player)));

    new_player->name = sfString_Create();
    sfString_SetText(new_player->name, name);
    new_player->life = 100;

    assert(new_player->weapons = (Weapon **) malloc(sizeof(Weapon*)));
    for (int i = 0; i < NB_MAX_WEAPONS; i++)
        new_player->weapons[i] = weapon_Create(i);

    new_player->weapons[current_weapon]->collected = true;      // Arme active
    new_player->nb_weapons = 1;
    new_player->current_weapon = current_weapon;                // Arme choisie dès le spawn

    /* TODO : Fonction donnant les coordonnées de spawn
    new_player->coord_x = ;                                     // Coordonnées de spawn
    new_player->coord_y = ;
    */

    new_player->m_coord_x = 0;
    new_player->m_coord_y = 0;

    new_player->frags = 0;
    new_player->killed = 0;

    stplayer_Create(new_player);

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

        free(player2destroy->weapons);
        free(player2destroy);
    }
}

void stplayer_Create(Player* player_)
{
    assert(player_);

    const char* name = sfString_GetText(player_->name);

    assert(player_->stripped = (stPlayer*) malloc(sizeof(stPlayer)));
    assert(player_->stripped->name = (char*) malloc(strlen(name) * sizeof(char)));
    strcpy(player_->stripped->name, name);

    player_->stripped->current_weapon = (sfUint8) player_->current_weapon;

    player_->stripped->coord_x = player_->coord_x;
    player_->stripped->coord_y = player_->coord_y;
    player_->stripped->coord_x = player_->m_coord_x;
    player_->stripped->coord_y = player_->m_coord_y;
}

void stplayer_Destroy(stPlayer* stplayer_)
{
    if (!stplayer_)
    {
        logging_Warning("stplayer_Destroy", "StPlayer object sent NULL");
        return;
    }
    free(stplayer_->name);
    free(stplayer_);
}

void stplayer_Update(Player* player_)
{
    if(!player_)
    {
        logging_Warning("stplayer_Update", "Player object sent NULL, aborting update");
        return;
    }

    player_->stripped->current_weapon = player_->current_weapon;
    player_->stripped->coord_x = player_->coord_x;
    player_->stripped->coord_y = player_->coord_y;
    player_->stripped->coord_x = player_->m_coord_x;
    player_->stripped->coord_y = player_->m_coord_y;
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
Bullet** player_WeaponShoot(Player* player_, int* nb_bullet)
{
    Bullet** new_bullet = NULL;

    if (player_->weapons[player_->current_weapon]->type == SHOTGUN)
    {
        assert(new_bullet = (Bullet**) malloc(SHOTGUN_SHRAPNELS * sizeof(Bullet*)));
        for (*nb_bullet = 0; *nb_bullet < SHOTGUN_SHRAPNELS; *nb_bullet++)
            bullet_Create(player_->player_id, player_->current_weapon);
    }
    else
        bullet_Create(player_->player_id, player_->current_weapon);

    player_->weapons[player_->current_weapon]->nb_curr_bullets--;

    return new_bullet;
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
