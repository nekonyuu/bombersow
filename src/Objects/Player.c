#include <assert.h>
#include <stdio.h>
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

    new_player->weapons[current_weapon]->collected = true;   // Arme active
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
    if(!player2destroy)
    {
        printf("Warning - player_Destroy : Player object sent NULL\n");
        return;
    }

    sfString_Destroy(player2destroy->name);
    for (int i = 0; i < NB_MAX_WEAPONS; i++)
        weapon_Destroy(player2destroy->weapons[i]);

    free(player2destroy->weapons);
    free(player2destroy);
}
