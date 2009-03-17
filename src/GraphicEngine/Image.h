#ifndef IMAGE_H
#define IMAGE_H

#include <SFML/Graphics.h>

#define BOUCLE -1
#define STOP 0

//Struct du gestionnaire d'image
typedef struct IMAGE
{
    sfImage **image_tab;        // Tableau stockant les images d'une map
    int image_nombre;           // Nombre d'images du tableau
}Image;

Image* image_Create();
void image_Destroy(Image*);

void image_Loader(Image*, char**, int);     //Loader d'image
sfImage* image_Get(Image*, int);            //Recuprer une sfImage a partir de son ID

//Struct pour les anumation
typedef struct ANIMATION
{
    sfSprite *sprite;           // Image convertie en Sprite de l'animation
    //Coordonnée de la premiere case
    int x;
    int y;
    //Taille d'une case
    int image_hauteur;
    int image_largeur;

    int nombre_image;
    int cur_image;              // Position dans l'animation
    int play;                   // Nombre de lectures à effectuer
    float fps;                  // Temps d'attente entre chaque image (en ms)

    sfClock *clock;             // Timer
} Animation;

//Animation* animation_Create();
Animation* animation_Create(sfImage*, int, int, int, int, int, int, int, float);
void animation_Destroy(Animation*);

void animation_Play(Animation*, int);
void animation_Draw(Animation*, sfRenderWindow*);
#endif

