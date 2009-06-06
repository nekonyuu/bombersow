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

#ifndef IMAGE_H
#define IMAGE_H

#include <SFML/Graphics.h>

#define BOUCLE -1
#define STOP 0

//Struct pour les anumation
typedef struct ANIMATION
{
    sfSprite *sprite;           // Image convertie en Sprite de l'animation
    //Coordonnée de la premiere case
    int x;
    int y;

    //Coordonnée de dessin
    int x_c;
    int y_c;

    //Taille d'une case
    int image_hauteur;
    int image_largeur;

    int nombre_image;
    int cur_image;              // Position dans l'animation
    int play;                   // Nombre de lectures à effectuer
    float fps;                  // Temps d'attente entre chaque image (en s)

    sfClock *clock;             // Timer
} Animation;

//Animation* animation_Create();
Animation* animation_Create(sfImage*, int, int, int, int, int, int, int, float);
void animation_Destroy(Animation*);

void animation_Play(Animation*, int);
void animation_Draw(Animation*, sfRenderWindow*);
void animation_SetPosition(Animation*, int, int);

// Struct des drawables
enum { SPRITE, ANIMATION }; //enum qui determine si il s'agit d'une image fixe ou d'une animation
typedef struct SPRITE
{

    sfSprite* sprite;
    Animation* animation;

    int type;           // Sprite/Animation

    int x;              //coordonnée
    int y;
    int largeur;        //taille
    int hauteur;

} Sprite;

Sprite* sprite_Create(int, int, sfImage*, Animation*);
void sprite_Destroy(Sprite*);
void sprite_SetPosition(Sprite*, int, int);
void sprite_Draw(sfRenderWindow*, Sprite*);
void sprite_SetColor(Sprite*, sfColor);
sfIntRect sprite_GetRect(Sprite*);
sfImage* sprite_GetImage(Sprite*);


// Struct du gestionnaire d'image
typedef struct IMAGE
{
    sfImage **image_tab;        // Tableau stockant les images d'une map
    int image_nombre;           // Nombre d'images du tableau


}Image;

Image* image_Create();
void image_Destroy(Image*);

void image_Loader(Image*, char**, int);     // Loader d'image
sfImage* image_Get(Image*, int);            // Récupérer une sfImage à partir de son ID

#endif

