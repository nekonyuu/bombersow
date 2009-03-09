#ifndef IMAGE_H
#define IMAGE_H
#include "SFML/Graphics.h"

#define BOUCLE -1
#define STOP 0

typedef struct IMAGE
{
    sfImage **image_tab;
    int image_nombre;
}Image;

Image * image_Create();
void image_Destroy(Image*);
void image_Loader(Image*, char**, int);
sfImage* image_Get(Image*, int);

typedef struct ANIMATION
{
    sfSprite *sprite;
    float x;
    float y;
    int image_hauteur;
    int image_largeur;
    int nombre_image;
    int cur_image;
    int play;
    int fps; //en ms
    sfClock *clock;
}Animation;

//Animation* animation_Create();
Animation* animation_Create(sfImage*, float, float, int, int, int, int, int, float);
void animation_Destroy(Animation*);
void animation_Play(Animation*, int);
void animation_Draw(Animation*, sfRenderWindow*);
#endif

