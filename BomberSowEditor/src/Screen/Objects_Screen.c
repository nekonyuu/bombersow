#include <assert.h>
#include "SFML/Graphics.h"
#include "Screen/Objects_Screen.h"

Object_Screen* object_screen_Create(sfRenderWindow* Game, sfImage* image, int x, int y, int largeur, int hauteur){

    Object_Screen* screen = NULL;
    assert(screen = (Object_Screen*) malloc(sizeof(Object_Screen)));

    screen->Game = Game;

    screen->image = NULL;


    screen->x = x;
    screen->y = y;

    screen->largeur = largeur;
    screen->hauteur = hauteur;

    screen->y_max = 0;
    screen->espace = 10;
    screen->x_cur = screen->espace+x;
    screen->y_cur = screen->espace+y;

    screen->page = 0;
    screen->last_id = 0;


    screen->background = sfSprite_Create();
    sfSprite_SetImage(screen->background, image);
    sfSprite_SetPosition(screen->background, x, y);

    return screen;

}

void object_screen_Load_Object(Object_Screen* screen, Image* image){

    screen->image = image;

}

void object_screen_Destroy(Object_Screen* screen){

    sfSprite_Destroy(screen->background);
    free(screen);
}

void object_screen_Draw(Object_Screen* screen){

    sfRenderWindow_DrawSprite(screen->Game, screen->background);

    sfSprite *sprite_temp = sfSprite_Create();
    if(screen->image != NULL){
        for(int i = 0; i < screen->image->image_nombre; i++){
            sfSprite_SetImage(sprite_temp, screen->image->image_tab[i]);
            sfSprite_SetPosition(sprite_temp, screen->x_cur, screen->y_cur);

            screen->x_cur += screen->espace+sfSprite_GetWidth(sprite_temp);

            sfRenderWindow_DrawSprite(screen->Game, sprite_temp);
        }
    }
    screen->x_cur = screen->espace+screen->x;
    screen->y_cur = screen->espace+screen->y;
    screen->y_max = 0;

    sfSprite_Destroy(sprite_temp);

}
