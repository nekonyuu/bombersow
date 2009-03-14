#ifndef WEAPON_H
#define WEAPON_H

#include <stdbool.h>
#include "SFML/Graphics.h"
#include "Objects/Bullet.h"

#define NB_MAX_WEAPONS 7
#define SHOTGUN_SHRAPNELS 7

// Liste des armes
enum WEAPON_TYPE { CROWBAR, ROCKET_LAUNCHER, GRENADES, SHOTGUN, MACHINEGUN, SNIPER, LASERGUN };

// Définition de la structure Arme
typedef struct WEAPON
{
    sfString *name;                 // Nom de l'arme
    int type;                       // ID de l'arme

    sfSprite *weapon_img;           // Image de l'arme
    sfSprite *bullet_img;           // Projectile

    int nb_max_bullets;             // Nombre de munitions max (-1 si infini)
    int nb_curr_bullets;            // Nombre de munitions restantes (-1 si infini)
    unsigned int damage;            // Dommages infligés par projectile

    unsigned int reload_latency;    // Temps de recharge (en millisecondes)
    unsigned int switch_latency;    // Temps avant possibilité de changement d'arme
    unsigned int respawn_time;      // Temps de réapparition de l'arme

    int range;                      // Portée de l'arme (-1 si infinie)
    unsigned int splash_radius;     // Distance de dommages collatéraux
    float splash_coef;              // Diminution des dommages selon la distance dans le cercle (dommage = (splash_coef) ^ distance par rapport au centre (en pixels) * dommages de base)
    float selfdamage_coef;          // Self-Damage (selfdamage * (splash_coef^distance du centre) * dmg)

    int proj_speed;                 // Vitesse des projectiles (-1 si instantané)
    unsigned int trajectory;        // Type de trajectoire (0 = Rectiligne, 1 = Parabole (Grenade), 2 = Spread (Shotgun)

    _Bool collected;                // Ramassée ? (utilisé seulement pour l'inventaire du joueur)
} Weapon;

Weapon armory[NB_MAX_WEAPONS];      // Armes du jeu en accès global

Weapon* weapon_Create(int);
void weapon_Destroy(Weapon*);
void armory_Create(Weapon*);
void armory_Destroy(Weapon*);

#endif
