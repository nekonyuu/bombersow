#ifndef STRIPPEDPLAYER_H
#define STRIPPEDPLAYER_H

#include "SFML/Graphics.h"

typedef struct STRIP_PLAYER {
    sfString* name;                 // Nom

    unsigned int *current_weapon;   // Arme courante

    // Emplacement sur la map
    float* coord_x;
    float* coord_y;

} StPlayer;

#endif
