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
    if (image_list_taille > 0)
    {
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
                free(image_list[i]);
            }
            free(image_list);
        }

    }
    data_Destroy(liste);
}

// Loader d'animations pour les map
void map_Loader_Animation(Animation** animation, unsigned int taille, Image* images, char* path)
{
    Data* data;

    data = data_Parser("[ANIMATION]", path);

    for (int i = 0; i < data->taille; i++)
    {
        int id = 0, id_image = 0, coord_x = 0, coord_y = 0, hauteur = 0, largeur = 0, nb_image = 0;
        float fps = 0.0;

        sscanf(data->data[i], "%d %d %d %d %d %d %d %f", &id, &id_image, &coord_x, &coord_y, &hauteur, &largeur, &nb_image, &fps);
        animation[i] = animation_Create(images->image_tab[id_image], coord_x, coord_y, hauteur, largeur, nb_image, 0, STOP, fps);
    }

    data_Destroy(data);
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
            if (strcmp(ent->d_name, ".") && strcmp(ent->d_name,".."))
                nombre_image++;

        if (nombre_image > 0)
        {
            assert(image_path = (char**) malloc(nombre_image * sizeof(char*)));
            for (i = 0; i < nombre_image; i++)
                assert(image_path[i] = (char*) malloc(255 * sizeof(char*)));

            rewinddir(rep);
            i = 0;
            while (i < nombre_image && (ent = readdir (rep)) != NULL)
                if (strcmp(ent->d_name, ".") && strcmp(ent->d_name,".."))
                {
                    strcpy(image_path[i], path);
                    strcpy(image_path[i], strcat(image_path[i], ent->d_name));
                    i++;
                }
        }
        image_Loader(image, image_path, nombre_image);

        for (i = 0; i < nombre_image; i++)
            free_secure(image_path[i]);
        free_secure(image_path);
    }

    closedir(rep);
}
