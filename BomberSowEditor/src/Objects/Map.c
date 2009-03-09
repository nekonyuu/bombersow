#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Objects/Objects.h"
#include "Objects/Map.h"
#include "file/file.h"

// Constructeur
Map* map_Create()
{
    Map* new_map = NULL;
    assert(new_map = (Map*) malloc(sizeof(Map)));

    new_map->background = NULL;
    new_map->objects = NULL;
    new_map->nb_objects = 0;


    new_map->animation = NULL;
    new_map->image = NULL;

    return new_map;
}

// Destructeur
void map_Destroy(Map* map2destroy)
{
    assert(map2destroy != NULL);

    sfSprite_Destroy(map2destroy->background);
    for (int i = 0; i < map2destroy->nb_objects; i++)
        object_Destroy(map2destroy->objects[i]);

    free(map2destroy);
}

// Loader d'image pour les map
void map_Loader_Image(Image* image_, char* path)
{

    Data *liste = data_Parser("[IMAGES]", path);

    char **image_list = NULL;
    int image_list_taille = 0;
    sscanf(liste->data[liste->taille-1], "%d", &image_list_taille);

    assert(image_list = (char**) malloc(image_list_taille*sizeof(char*))); //Allocation de la mémoire
    for (int i = 0; i < image_list_taille; i++)
    {
        assert(image_list[i] = (char*) malloc(sizeof(char)));
    }

    for (int i = 0; i < liste->taille; i++)
    {
        int id;
        char path[100];
        sscanf(liste->data[i], "%d %s", &id, path);
        image_list[id] = path;
    }

    image_Loader(image_, image_list, image_list_taille);

    if (image_list != NULL)
    {
        for (int i = 0; i < image_list_taille; i++)
        {
            free(image_list[i]);
        }
        free(image_list);
    }

    data_Destroy(liste);
}
