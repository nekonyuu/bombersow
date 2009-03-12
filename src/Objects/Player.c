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

    /* TODO : Fonction random de coordonnées de spawn
    new_player->coord_x = ;                                     // Coordonnées de spawn
    new_player->coord_y = ;
    */

    new_player->frags = 0;
    new_player->killed = 0;

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

// Fonction qui gère le saut du joueur, TODO : Trajectoire lors du saut, vecteur force
void player_Jump(Player* player_)
{
    assert(player_)
    if (player_->jump == NO_JUMP)
    {
        player_->jump = SIMPLE_JUMP;
    }
    else if (player_->jump == SIMPLE_JUMP)
    {
        player_->jump = NO_JUMP;
    }
}
