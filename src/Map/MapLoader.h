/*
    GPL v3 Licence :
    Bombersow is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bombersow is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bombersow.  If not, see <http://www.gnu.org/licenses/>.


    Creative Commons BY-NC-SA :
    This work is licensed under the Creative Commons Attribution-Noncommercial-Share Alike 3.0 Unported License.
    To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter
    to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.

*/

#ifndef MAPLOADER_H
#define MAPLOADER_H

#include "GraphicEngine/Image.h"
#include "Objects/GameObjects.h"

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

void dossier_Read_Image(Image* image, char* path);

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
TYPE    X       Y      PLAN     ID_ANIMATION    ID_IMAGE        LARGEUR     HAUTEUR     [PARAMETRE OPTIONNEL]
int     int     int    int      int             int             int         int         [PARA]

*/
