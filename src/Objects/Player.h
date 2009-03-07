#ifndef PLAYER_H
#define PLAYER_H

#include "Weapon.h"

typedef struct PLAYER
{
    sfString *name;         // Nom du joueur
    unsigned int life;      // Vie restante

    Weapon current_weapon;  // Arme courante

    // Emplacement sur la map
    float coord_x;
    float coord_y;

    unsigned int frags;     // Nombre de tués
    unsigned int killed;    // Nombre de morts
} Player;

Player* player_Create(char*, Weapon*);
void player_Destroy(Player*);

#endif
