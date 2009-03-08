#ifndef WEAPON_H
#define WEAPON_H

#include <stdbool.h>

#include "Objects/Objects.h"
#include "Objects/Bullet.h"

#define NB_MAX_WEAPONS 7

// D�finition de la structure Arme
typedef struct WEAPON
{
    sfString *name;                 // Nom de l'arme

    Object *weapon_img;             // Image de l'arme
    Bullet *bullet;                 // Projectile

    int nb_max_bullets;             // Nombre de munitions max (-1 si infini)
    int nb_curr_bullets;            // Nombre de munitions restantes (-1 si infini)
    unsigned int damage;            // Dommages inflig�s
    unsigned int reload_latency;    // Temps de recharge (en millisecondes)
    int range;                      // Port�e de l'arme (-1 si infinie)
    unsigned int splash_radius;     // Distance de dommages collat�raux
    float splash_coef;              // Diminution des dommages selon la distance dans le cercle (dommage = (splash_coef) ^ distance par rapport au centre (en pixels) * dommages de base)
    float selfdamage_coef;          // Self-Damage (selfdamage * (splash_coef^distance du centre) * dmg)
    unsigned int proj_speed;        // Vitesse des projectiles

    _Bool collected;                // Ramass�e ? (utilis� seulement pour l'inventaire du joueur)
} Weapon;

Weapon armory[NB_MAX_WEAPONS];      // Armes du jeu en acc�s global

Weapon* weapon_Create(int);
void weapon_Destroy(Weapon*);
void armory_Create(Weapon*);
void armory_Destroy(Weapon*);

#endif
