#ifndef WEAPON_H
#define WEAPON_H

#include <SFML/

// D�finition de la structure Arme
typedef struct WEAPON
{
    unsigned int id;
    char* name;
    unsigned int nb_bullets;
    unsigned int bullet_id;
    unsigned int damage;
    unsigned int bullet_speed;
    unsigned float reload_latency;
} Weapon;

#endif
