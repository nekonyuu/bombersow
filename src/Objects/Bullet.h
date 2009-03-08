#ifndef BULLET_H
#define BULLET_H

#include "SFML/Graphics.h"

typedef struct BULLET
{
    unsigned int bullet_type;   // Type de balles
    unsigned int trajectory;    // Type de trajectoire
    sfImage* draw_image;        // Image de la balle
} Bullet;

Bullet* bullet_Create(unsigned int);
void bullet_Destroy(Bullet*);

#endif
