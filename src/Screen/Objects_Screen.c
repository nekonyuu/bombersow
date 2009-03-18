#include <assert.h>
#include "SFML/Graphics.h"
#include "Screen/Objects_Screen.h"

Object_Screen* object_screen_Create(sfRenderWindow* Game, sfImage* image, int x, int y, int largeur, int hauteur)
{

    Object_Screen* screen = NULL;
    assert(screen = (Object_Screen*) malloc(sizeof(Object_Screen)));

    screen->Game = Game;

    screen->image = NULL;

    screen->animation = NULL;
    screen->animation_nombre = 0;

    screen->type_affichage = 0;

    screen->x = x;
    screen->y = y;

    screen->largeur = largeur;
    screen->hauteur = hauteur;

    screen->y_max = 0;
    screen->espace = 10;
    screen->x_cur = screen->espace+x;
    screen->y_cur = screen->espace+y;

    screen->y_pos = 0;


    screen->background = sfSprite_Create();
    sfSprite_SetImage(screen->background, image);
    sfSprite_SetPosition(screen->background, x, y);

    return screen;

}

void object_screen_Load_Object(Object_Screen* screen, Image* image)
{

    screen->image = image;

}

void object_screen_Load_Animation(Object_Screen* screen, Animation** animation, int nombre)
{

    screen->animation = animation;
    screen->animation_nombre = nombre;

}

void object_screen_Destroy(Object_Screen* screen)
{

    for (int i = 0; i < screen->animation_nombre; i++)
        animation_Destroy(screen->animation[i]);

    free(screen->animation);
    screen->animation = NULL;

    sfSprite_Destroy(screen->background);
    free(screen);
}

void object_screen_Draw(Object_Screen* screen)
{

    sfRenderWindow_DrawSprite(screen->Game, screen->background);

    sfSprite *sprite_temp = sfSprite_Create();


    screen->x_cur = screen->espace+screen->x;
    screen->y_cur = screen->espace+screen->y;

    screen->y_max = 0;

    if (screen->image != NULL)
    {

        for (int i = 0; i < screen->image->image_nombre; i++)
        {
            sfSprite_Destroy(sprite_temp);
            sprite_temp = sfSprite_Create();
            sfSprite_SetImage(sprite_temp, screen->image->image_tab[i]);

            if (screen->x_cur+sfSprite_GetWidth(sprite_temp) > screen->largeur)
            {
                screen->x_cur = screen->x+screen->espace;
                screen->y_cur = screen->espace+screen->y_max;
            }

            sfSprite_SetPosition(sprite_temp, screen->x_cur, screen->y_cur);

            screen->y_max = (screen->y_cur+sfSprite_GetHeight(sprite_temp) > screen->y_max) ? screen->y_cur+sfSprite_GetHeight(sprite_temp) : screen->y_max;
            screen->x_cur += screen->espace+sfSprite_GetWidth(sprite_temp);

            sfRenderWindow_DrawSprite(screen->Game, sprite_temp);
        }
    }

    sfSprite_Destroy(sprite_temp);

}

int object_screen_Click(Object_Screen* screen, int mouse_x, int mouse_y)
{

    sfIntRect cadre_screen = {screen->x, screen->y, screen->x+screen->largeur, screen->y+screen->hauteur};
    if (sfIntRect_Contains(&cadre_screen, mouse_x, mouse_y))
    {
        sfSprite *sprite_temp = sfSprite_Create();

        screen->x_cur = screen->espace + screen->x;
        screen->y_cur = screen->espace + screen->y;

        screen->y_max = 0;

        if (screen->image != NULL)
        {
            for (int i = 0; i < screen->image->image_nombre; i++)
            {
                sprite_temp = sfSprite_Create();
                sfSprite_SetImage(sprite_temp, screen->image->image_tab[i]);

                if (screen->x_cur+sfSprite_GetWidth(sprite_temp) > screen->largeur)
                {
                    screen->x_cur = screen->x+screen->espace;
                    screen->y_cur = screen->espace+screen->y_max;
                }

                sfSprite_SetPosition(sprite_temp, screen->x_cur, screen->y_cur);

                sfIntRect cadre_screen = {screen->x_cur, screen->y_cur, screen->x_cur+sfSprite_GetWidth(sprite_temp), screen->y_cur+sfSprite_GetHeight(sprite_temp)};
                if (sfIntRect_Contains(&cadre_screen, mouse_x, mouse_y))
                    return i;

                screen->y_max = (screen->y_cur+sfSprite_GetHeight(sprite_temp) > screen->y_max) ? screen->y_cur+sfSprite_GetHeight(sprite_temp) : screen->y_max;
                screen->x_cur += screen->espace+sfSprite_GetWidth(sprite_temp);

            }
        }
        sfSprite_Destroy(sprite_temp);
        return -1;
    }
    else
        return -1;
}
