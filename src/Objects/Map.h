#ifndef MAP_H
#define MAP_H

#include "SFML/Graphics.h"
#include "Objects/Objects.h"
#include "Objects/Player.h"

#define NB_MAX_PLAYERS 4

typedef struct MAP
{
    sfImage* background;        // Arrière-plan

    Object** objects;           // Tableau des objets de la map
    unsigned int nb_objects;    // Nombre d'objets sur la map

    Player** player_list;       // Liste des joueurs de la map
    unsigned int nb_players;    // Nombre de joueurs connectés sur la map
} Map;

Map* map_Create();
void map_Destroy();
void map_Loader(Map*, char*);


#endif

//Structure des Fichier .MAP
/*
[BACKGROUND]
PATH
char
[OBJECTS]
TYPE    ID      X       Y      COLLISION
int     int     int    int

*/
