#ifndef FILE_H
#define FILE_H

#include <dirent.h>
#include "SFML/Graphics.h"
#include "GraphicEngine/Image.h"

//Structure pour chargée les données (a renommer/modifier/déplacer sans doute)
typedef struct DATA
{
    char** data;
    int taille;
    int seek;
}Data;

Data* data_Create();
void data_Destroy(Data*);
Data* data_Parser(char *type, char* path); //Parser qui recupere un char des champs correspondant a un type

void dossier_Read_Image(Image* image, char* path);
#endif
