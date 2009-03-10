#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "file/file.h"


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

    if (fichier == NULL)     //Si on a pas pu ouvrir le fichier on renvoi NULL
    {
        printf("Erreur lors de l'ouverture du fichier %s\n", path);
        return NULL;
    }

    char chaine[100];
    int bool_ = 0; //Permet de bien se placer

    Data *data_ = data_Create();

    int taille_type = strlen(type);

    while (fgets(chaine, 100, fichier) != NULL)    //Boucle pour lire ligne
    {

        if (strlen(chaine) != 0)                                //Si la chaine n'est pas vide
        {
            if (!bool_ && strncmp(type, chaine,taille_type) == 0)     //On détermine si on est au bon endroit
            {
                bool_ = 1;
                data_->seek = ftell(fichier); //Permet de placer le seek à la ligne suivante pour évitez de reparcourir tout le fichier
            }
            else if (chaine[0] == '[')
            {
                break;
            }
            else
            {
                if (chaine[0] != '#')
                    data_->taille++;    //Incrémente le nombre d'element
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



void dossier_Read_Image(Image* image, char* path){

    DIR *rep = opendir (path);


    int i = 0;

    int nombre_image = 0;
    char **image_path = NULL;

    if (rep != NULL){
        struct dirent *ent;

        while ((ent = readdir (rep)) != NULL){
            if(strcmp(ent->d_name, ".") && strcmp(ent->d_name,"..")){
                nombre_image++;
            }
        }

        assert(image_path = (char**) malloc(nombre_image*sizeof(char*)));
        for(i = 0; i < nombre_image; i++)
            assert(image_path[i] = (char*) malloc(100*sizeof(char*)));

        rewinddir(rep);
        i = 0;
        while ((ent = readdir (rep)) != NULL){
            if(strcmp(ent->d_name, ".") && strcmp(ent->d_name,"..")){
                strcpy(image_path[i], path);
                strcpy(image_path[i], strcat(image_path[i], ent->d_name));
                i++;
            }
        }

        closedir (rep);
    }

    image_Loader(image, image_path, nombre_image);

    for(i = 0; i < nombre_image; i++)
        free(image_path[i]);
    free(image_path);
}
