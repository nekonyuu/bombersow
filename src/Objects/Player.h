#ifndef PLAYER_H
#define PLAYER_H

#include "Objects/Weapon.h"

typedef struct PLAYER
{
    sfString *name;                 // Nom du joueur
    unsigned int life;              // Vie restante

    Weapon **weapons;               // Armes du joueur
    unsigned int nb_weapons;        // Nombre d'armes
    unsigned int current_weapon;    // Arme courante

    // Emplacement sur la map
    float coord_x;
    float coord_y;

    unsigned int frags;             // Nombre de tués
    unsigned int killed;            // Nombre de morts
} Player;

Player* player_Create(char*, unsigned int);
void player_Destroy(Player*);

#endif
