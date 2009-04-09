#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "BaseSystem/Logging.h"
#include "Map/MapLoader.h"

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
        free_secure(data_->data[i]);
    }

    free_secure(data_->data);
    data_->data = NULL;

    free_secure(data_);
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
            if (!bool_ && strncmp(type, chaine,taille_type) == 0) // On d�termine si on est au bon endroit
            {
                bool_ = 1;
                data_->seek = ftell(fichier);    // Permet de placer le seek � la ligne suivante pour �vitez de reparcourir tout le fichier
            }
            else if (chaine[0] == '[')
            {
                break;
            }
            else
            {
                if (chaine[0] != '#')
                    data_->taille++;            // Incr�mente le nombre d'element
            }
        }

    }


    if (data_->taille != 0)
    {
        assert(data_->data = (char**) malloc(data_->taille*sizeof(char*))); //Allocation de la m�moire
        for (int i = 0; i < data_->taille; i++)
        {
            assert(data_->data[i] = (char*) malloc(100*sizeof(char)));
        }
    }

    fseek(fichier, data_->seek, 0);
    fgets(chaine, 100, fichier); //Corrige les erreurs associ� au fin de ligne lors du replacement

    bool_ = 0;
    int i = 0;
    while (fgets(chaine, 100, fichier) != NULL)    //Boucle pour lire ligne
    {

        if (!bool_ && strncmp(type, chaine,taille_type) == 0)     //On d�termine si on est au bon endroit
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
    if(image_list_taille < 1)
        return;
    assert(image_list = (char**) malloc(image_list_taille * sizeof(char*))); //Allocation de la m�moire
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
            free_secure(image_list[i]);
        }
        free_secure(image_list);
    }

    data_Destroy(liste);
}

void dossier_Read_Image(Image* image, char* path)
{

    DIR *rep = opendir(path);


    int i = 0;

    int nombre_image = 0;
    char **image_path = NULL;

    if (rep != NULL)
    {
        struct dirent *ent;

        while ((ent = readdir(rep)) != NULL)
        {
            if (strcmp(ent->d_name, ".") && strcmp(ent->d_name,".."))
            {
                nombre_image++;
            }
        }

        assert(image_path = (char**) malloc(nombre_image*sizeof(char*)));
        for (i = 0; i < nombre_image; i++)
            assert(image_path[i] = (char*) malloc(100*sizeof(char*)));

        rewinddir(rep);
        i = 0;
        while ((ent = readdir (rep)) != NULL)
        {
            if (strcmp(ent->d_name, ".") && strcmp(ent->d_name,".."))
            {
                strcpy(image_path[i], path);
                strcpy(image_path[i], strcat(image_path[i], ent->d_name));
                i++;
            }
        }

        closedir (rep);
    }

    image_Loader(image, image_path, nombre_image);

    for (i = 0; i < nombre_image; i++)
        free_secure(image_path[i]);
    free_secure(image_path);
}
