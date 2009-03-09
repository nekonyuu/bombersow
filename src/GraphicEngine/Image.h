#ifndef IMAGE_H
#define IMAGE_H
#include "SFML/Graphics.h"

#define BOUCLE -1
#define STOP 0

//Struct du gestionnaire d'image
typedef struct IMAGE
{
    sfImage **image_tab;    //Tableau de sfimage
    int image_nombre;       //Taille du tableau
}Image;

Image * image_Create();
void image_Destroy(Image*);

void image_Loader(Image*, char**, int);     //Loader d'image
sfImage* image_Get(Image*, int);            //Recuprer une sfImage a partir de son ID

//Struct pour les anumation
typedef struct ANIMATION
{
    sfSprite *sprite;

    //Coordonnée de la premiere case
    float x;
    float y;

    //Taille d'une case
    int image_hauteur;
    int image_largeur;

    int nombre_image;

    int cur_image;

    int play;

    float fps; //en s

    sfClock *clock;//Timer
}Animation;

//Animation* animation_Create();
Animation* animation_Create(sfImage*, float, float, int, int, int, int, int, float);
void animation_Destroy(Animation*);

void animation_Play(Animation*, int);
void animation_Draw(Animation*, sfRenderWindow*);
#endif

