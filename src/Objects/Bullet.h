#ifndef BULLET_H
#define BULLET_H

#include "SFML/Graphics.h"

typedef struct BULLET
{
    unsigned int owner;         // Tireur (utile pour le self-damage)
    unsigned int bullet_type;   // Type de balles
    unsigned int damage;        // Dommages infligés
    unsigned int trajectory;    // Type de trajectoire (0 = Rectiligne, 1 = Parabole (Grenade), 2 = Spread (Shotgun)
    sfSprite* draw_image;       // Image de la balle (Peut être NULL, balle invisible)
} Bullet;

Bullet* bullet_Create(unsigned int, unsigned int);
void bullet_Destroy(Bullet*);

#endif
