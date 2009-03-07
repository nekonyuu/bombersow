#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.h>

#include "Objects.h"

typedef struct MAP
{
    sfImage* background;        // Arrière-plan
    Object** objects;            // Tableau des objets de la map
    unsigned int nb_objects;    // Nombre d'objets sur la map
} Map;

Map* map_Create();
void map_Destroy();

#endif
