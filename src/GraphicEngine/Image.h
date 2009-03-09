#ifndef IMAGE_H
#define IMAGE_H

#include "SFML/Graphics.h"

#define BOUCLE -1
#define STOP 0

typedef struct IMAGE
{
    sfImage **image_tab;        // Tableau stockant les images d'une map
    int image_nombre;           // Nombre d'images du tableau
}Image;

Image* image_Create();
void image_Destroy(Image*);
void image_Loader(Image*, char**, int);
sfImage* image_Get(Image*, int);

typedef struct ANIMATION
{
    sfSprite *sprite;           // Image convertie en Sprite de l'animation
    float x;                    // Coordonn�es
    float y;
    int image_hauteur;          // Dimensions
    int image_largeur;
    int nombre_image;
    int cur_image;              // Position dans l'animation
    int play;                   // Nombre de lectures � effectuer
    int fps;                    // Temps d'attente entre chaque image (en ms)
    sfClock *clock;             // Timer
} Animation;

//Animation* animation_Create();
Animation* animation_Create(sfImage*, float, float, int, int, int, int, int, float);
void animation_Destroy(Animation*);
void animation_Play(Animation*, int);
void animation_Draw(Animation*, sfRenderWindow*);
#endif

