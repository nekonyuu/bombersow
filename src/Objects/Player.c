#include <assert.h>

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

    for(int i = 0; i < NB_MAX_WEAPONS; i++)
        new_player->weapons[i] = armory[i];

    new_player->weapons[current_weapon].collected = true;   // Arme active
    new_player->nb_weapons = 1;
    new_player->current_weapon = current_weapon;            // Arme choisie dès le spawn

    /* TODO : Fonction random de coordonnées de spawn
    new_player->coord_x = ;                                 // Coordonnées de spawn
    new_player->coord_y = ;
    */

    new_player->frags = 0;
    new_player->killed = 0;

    return new_player;
}

// Destructeur
void player_Destroy(Player* player2destroy)
{
    sfString_Destroy(player2destroy->name);

    free(player2destroy);
}
