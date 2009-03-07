#ifndef WEAPON_H
#define WEAPON_H

#define NB_WEAPONS 5

#include "Objects.h"

// Définition de la structure Arme
typedef struct WEAPON
{
    char* name;                     // Nom de l'arme

    Object weapon_img;              // Image de l'arme
    Object bullet;                  // Image des munitions

    unsigned int nb_max_bullets;    // Nombre de munitions max
    unsigned int nb_curr_bullets;   // Nombre de munitions restantes
    unsigned int damage;            // Dommages infligés
    float reload_latency;           // Temps de recharge
} Weapon;

#endif
