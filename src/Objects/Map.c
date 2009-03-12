#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BaseSystem/Logging.h"
#include "Objects/Objects.h"
#include "Objects/Map.h"

// Constructeur
Map* map_Create()
{
    Map* new_map = NULL;
    assert(new_map = (Map*) malloc(sizeof(Map)));

    new_map->background = NULL;
    new_map->objects = NULL;
    new_map->nb_objects = 0;
    new_map->nb_players = 0;

    assert(new_map->player_list = (Player**) malloc(sizeof(Player*)));
    for (int i = 0; i < NB_MAX_PLAYERS; i++)
        new_map->player_list[i] = NULL;

    new_map->animation = NULL;
    new_map->image = NULL;

    return new_map;
}

// Destructeur
void map_Destroy(Map* map2destroy)
{
    if (!map2destroy)
        logging_Warning("map_Destroy", "Map object sent NULL");
    else
    {
        sfSprite_Destroy(map2destroy->background);
        for (int i = 0; i < map2destroy->nb_objects; i++)
            object_Destroy(map2destroy->objects[i]);

        free(map2destroy);
    }
}

Data* data_Create()
{
    Data *data_ = NULL;
    assert(data_ = (Data*) malloc(sizeof(Data)));
    data_->taille = 0;
    data_->seek = 0;
    data_->data = NULL;

    return data_;
}

void data_Destroy(Data *data_)
{

    for (int i = 0; i < data_->taille; i++)
    {
        free(data_->data[i]);
    }
    free(data_->data);
    data_->data = NULL;

    free(data_);
    data_ = NULL;
}

Data* data_Parser(char *type, char* path)
{
    FILE* fichier = NULL;
    fichier = fopen(path, "r");

    if (fichier == NULL)                        // Si on a pas pu ouvrir le fichier on renvoi NULL
    {
        char str[200];
        strcat(str, "File ");
        strcat(str, path);
        strcat(str, " doesn't exist");
        logging_Warning("data_Parser", str);
        return NULL;
    }

    char chaine[100];
    int bool_ = 0;                              // Permet de bien se placer

    Data *data_ = data_Create();

    int taille_type = strlen(type);

    while (fgets(chaine, 100, fichier) != NULL) // Boucle pour lire ligne
    {

        if (strlen(chaine) != 0)                // Si la chaine n'est pas vide
        {
            if (!bool_ && strncmp(type, chaine,taille_type) == 0) // On détermine si on est au bon endroit
            {
                bool_ = 1;
                data_->seek = ftell(fichier);    // Permet de placer le seek à la ligne suivante pour évitez de reparcourir tout le fichier
            }
            else if (chaine[0] == '[')
            {
                break;
            }
            else
            {
                if (chaine[0] != '#')
                    data_->taille++;            // Incrémente le nombre d'element
            }
        }

    }


    if (data_->taille != 0)
    {
        assert(data_->data = (char**) malloc(data_->taille*sizeof(char*))); //Allocation de la mémoire
        for (int i = 0; i < data_->taille; i++)
        {
            assert(data_->data[i] = (char*) malloc(100*sizeof(char)));
        }
    }

    fseek(fichier, data_->seek, 0);
    fgets(chaine, 100, fichier); //Corrige les erreurs associé au fin de ligne lors du replacement

    bool_ = 0;
    int i = 0;
    while (fgets(chaine, 100, fichier) != NULL)    //Boucle pour lire ligne
    {

        if (!bool_ && strncmp(type, chaine,taille_type) == 0)     //On détermine si on est au bon endroit
            bool_ = 1;
        else if (chaine[0] == '[')
            break;
        else
        {
            strcpy(data_->data[i], chaine);
            i++;
        }
    }

    fclose(fichier);

    return data_;
}

// Loader d'image pour les map
void map_Loader_Image(Image* image_, char* path)
{

    Data *liste = data_Parser("[IMAGES]", path);

    char **image_list = NULL;
    int image_list_taille = 0;
    sscanf(liste->data[liste->taille-1], "%d", &image_list_taille);

    assert(image_list = (char**) malloc(image_list_taille * sizeof(char*))); //Allocation de la mémoire
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
