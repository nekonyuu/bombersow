#ifndef MAPLOADER_H
#define MAPLOADER_H

#include "GraphicEngine/Image.h"
#include "Objects/Map.h"

// Structure pour chargée les données
typedef struct DATA
{
    char** data;                            // Stocke les chemins des données de map
    int taille;                             // Taille du tableau de char
    int seek;                               // Position
} Data;

void map_Loader_Image(Image*, char*);       // Loader pour les images
Object** map_Loader_Object(char*);          // Loader pour les objects
void map_Loader(Map*, char*);               // Loader de la map

Data* data_Create();
void data_Destroy(Data*);
Data* data_Parser(char*, char*);            // Parser qui recupere un char des champs correspondant a un type

#endif

// Structure des Fichier .MAP
/*
[IMAGES]
ID PATH
0=image par défault

[ANIMATION]
ID      ID_IMAGE    X       Y       HAUTEUR     LARGEUR     NOMBRE_IMAGE    FPS
int     int         int     int     int         int         int             float

[SOUNDS]
ID PATH
0=sounds par défaut

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
