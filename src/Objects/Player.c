#include <assert.h>

#include "Player.h"

// Constructeur
Player* init_Player(char* name, Weapon* current_weapon)
{
    Player* new_player = NULL;

    assert(new_player = malloc(sizeof(Player)));

    new_player->name = sfString_Create();
    sfString_SetText(new_player->name, name);
    new_player->life = 100;

    /* TODO
    if(current_weapon == NULL)
        new_player->current_weapon = ;
    else
        new_player->current_weapon = current_weapon;

    new_player->coord_x = ;
    new_player->coord_y = ;
    */

    new_player->frags = 0;
    new_player->killed = 0;

    return new_player;
}
