#ifndef MAP_H
#define MAP_H

#include "SFML/Graphics.h"
#include "Objects/Objects.h"
#include "Objects/Player.h"

#define NB_MAX_PLAYERS 4

typedef struct MAP
{
    unsigned int mapId;         // ID de la map (transmission réseau)
    sfSprite* background;       // Arrière-plan

    Image** image;              // Tableau pour les images de la map
    Animation** animation;      // Tableau pour les animations

    Object** objects;           // Tableau des objets de la map
    unsigned int nb_objects;    // Nombre d'objets sur la map

    Player** player_list;       // Liste des joueurs de la map
    unsigned int nb_players;    // Nombre de joueurs connectés sur la map
} Map;

Map* map_Create();
void map_Destroy();
void map_Loader(Map*, char*);         //Loader de la map

/*A renommer et déplacer (pas spécifique au fichier map)*/
void map_Loader_Image(Image*, char*);      //Loader pour les images
Object** map_Loader_Object(char*);    //Loader pour les objects
/********************************************************/

//Structure pour chargée les données (a renommer/modifier/déplacer sans doute)
typedef struct DATA
{
    char** data;
    int taille;
    int seek;
}Data;

Data* data_Create();
void data_Destroy(Data*);
Data* data_Parser(char*, char*); //Parser qui recupere un char des champs correspondant a un type

#endif

//Structure des Fichier .MAP
/*
[IMAGES]
ID PATH
0=image par défault

[ANIMATION]
ID      ID_IMAGE    X       Y       HAUTEUR     LARGEUR     NOMBRE_IMAGE    FPS
int     int         int     int     int         int         int             float

[SOUNDS]
ID PATH
0=sounds par défault

[BACKGROUND]
ID_IMAGE
int

[OBJECTS]
TYPE    ID_ANIMATION    ID_IMAGE        LARGEUR     HAUTEUR     [PARAMETRE OPTIONNEL]
int     int             int             int         int         [PARA]
//Si id_animation = -1, image fixe id_image != -1
//si id_animation !=-1, id_image = -1

[ELEMENTS]
ID_OBJECT      X       Y      PLAN     [PARAMETRE OPTIONNEL]
int            int    int     int      [para en plus suivant le type]

*/
