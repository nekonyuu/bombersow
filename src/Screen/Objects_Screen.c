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

#include <assert.h>
#include <GL/glew.h>
#include "SFML/Graphics.h"
#include "Gui/Gui.h"
#include "Objects/Editor.h"
#include "Screen/Objects_Screen.h"

Object_Screen* object_screen_Create(sfRenderWindow* Game, sfImage* image, int x, int y, int largeur, int hauteur, Image* image_obj, Editor* editor)
{

    Object_Screen* screen = NULL;
    assert(screen = (Object_Screen*) malloc(sizeof(Object_Screen)));

    screen->editor = editor;

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

    screen->valeur = 0;


    screen->background = sfSprite_Create();
    sfSprite_SetImage(screen->background, image);
    sfSprite_SetPosition(screen->background, x, y);

    screen->gui = gui_Create();

    Widget_slide* slide = widget_slide_Create(x+largeur-16, y+8, 11, hauteur-16, 1, sfColor_FromRGB(85, 137, 199), image_Get(image_obj, 3), image_Get(image_obj, 5), image_Get(image_obj, 4));
    gui_Add_Slide(screen->gui, slide);

    slide = widget_slide_Create(x+largeur-16, y+8, 11, hauteur-16, 1, sfColor_FromRGB(85, 137, 199), image_Get(image_obj, 3), image_Get(image_obj, 5), image_Get(image_obj, 4));
    gui_Add_Slide(screen->gui, slide);


    return screen;

}

void object_screen_Load_Object(Object_Screen* screen, Image* image)
{

    screen->image = image;

    if (screen->image != NULL)
    {

        assert(screen->sprite = (sfSprite**) malloc(image->image_nombre*sizeof(sfSprite*)));
        for(int i = 0; i < image->image_nombre; i++)
            screen->sprite[i] = sfSprite_Create();

        screen->sprite_nombre = image->image_nombre;

        screen->x_cur = screen->espace+screen->x;
        screen->y_cur = screen->espace+screen->y;

        screen->y_max = 0;

        for (int i = 0; i < image->image_nombre; i++)
        {

            sfSprite_SetImage(screen->sprite[i], image->image_tab[i]);

            if (screen->x_cur+sfSprite_GetWidth(screen->sprite[i]) > screen->largeur)
            {
                screen->x_cur = screen->x+screen->espace;
                screen->y_cur = screen->espace+screen->y_max;
            }

            sfSprite_SetPosition(screen->sprite[i], screen->x_cur, screen->y_cur);

            screen->y_max = (screen->y_cur+sfSprite_GetHeight(screen->sprite[i]) > screen->y_max) ? screen->y_cur+sfSprite_GetHeight(screen->sprite[i]) : screen->y_max;
            screen->x_cur += screen->espace+sfSprite_GetWidth(screen->sprite[i]);
        }

        sprite_Destroy(screen->editor->selected_sprite);
        screen->editor->selected_sprite = sprite_Create(0, 0, image_Get(screen->image, 0), NULL);
        screen->editor->selected_id = 0;
        screen->editor->selected_type = 0;
    }

    widget_slide_SetNbrVal(screen->gui->widget_slide[0], (screen->y_max-screen->y)/(screen->hauteur-(2*screen->espace))+1);
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

    for (int i = 0; i < screen->sprite_nombre; i++)
        sfSprite_Destroy(screen->sprite[i]);

    free(screen->sprite);
    screen->sprite = NULL;

    gui_Destroy(screen->gui);

    sfSprite_Destroy(screen->background);
    free(screen);
}

void object_screen_Draw(Object_Screen* screen)
{

    sfRenderWindow_DrawSprite(screen->Game, screen->background);

    glEnable (GL_SCISSOR_TEST);
    glScissor(screen->x, 800-screen->hauteur-screen->y+screen->espace, screen->largeur-40,  screen->hauteur-(2*screen->espace));

    if(screen->type_affichage){

        for (int i = 0; i < screen->animation_nombre; i++)
        {

            if(screen->valeur != screen->gui->widget_slide[1]->valeur){
                int co_move = (screen->valeur < screen->gui->widget_slide[1]->valeur) ? -(screen->hauteur-(2*screen->espace)) : screen->hauteur-(2*screen->espace);
                sfSprite_Move(screen->animation[i]->sprite, 0, co_move);
            }
            animation_Draw(screen->animation[i], screen->Game);

        }
        screen->valeur = screen->gui->widget_slide[1]->valeur;

        glDisable (GL_SCISSOR_TEST);

        widget_slide_Draw(screen->Game, screen->gui->widget_slide[1]);

    }else{
        for (int i = 0; i < screen->sprite_nombre; i++)
        {

            if(screen->valeur != screen->gui->widget_slide[0]->valeur){
                int co_move = (screen->valeur < screen->gui->widget_slide[0]->valeur) ? -(screen->hauteur-(2*screen->espace)) : screen->hauteur-(2*screen->espace);
                sfSprite_Move(screen->sprite[i], 0, co_move);
            }
            sfRenderWindow_DrawSprite(screen->Game, screen->sprite[i]);

        }
        screen->valeur = screen->gui->widget_slide[0]->valeur;

        glDisable (GL_SCISSOR_TEST);

        widget_slide_Draw(screen->Game, screen->gui->widget_slide[0]);
    }


}

void object_screen_Click(Object_Screen* screen, int mouse_x, int mouse_y)
{

    gui_Click(screen->gui, mouse_x, mouse_y);

    sfIntRect cadre_screen = {screen->x, screen->y+screen->espace, screen->x+screen->largeur-40, screen->y+screen->hauteur-(2*screen->espace)};

    if (sfIntRect_Contains(&cadre_screen, mouse_x, mouse_y))
    {

        if(screen->type_affichage){
            for (int i = 0; i < screen->animation_nombre; i++)
            {

                sfIntRect cadre_screen = {screen->animation[i]->x_c, screen->animation[i]->y_c, screen->animation[i]->x_c+screen->animation[i]->image_largeur, screen->animation[i]->y_c+screen->animation[i]->image_hauteur};
                if (sfIntRect_Contains(&cadre_screen, mouse_x, mouse_y)){

                    sprite_Destroy(screen->editor->selected_sprite);
                    Animation* temp_anim = animation_Create(
                                                    sfSprite_GetImage(screen->animation[i]->sprite),
                                                    screen->animation[i]->x,
                                                    screen->animation[i]->y,
                                                    screen->animation[i]->image_hauteur,
                                                    screen->animation[i]->image_largeur,
                                                    screen->animation[i]->nombre_image,
                                                    0, BOUCLE,
                                                    screen->animation[i]->fps);

                    screen->editor->selected_sprite = sprite_Create(mouse_x, mouse_y, NULL, temp_anim);

                    screen->editor->selected_id = i;
                    screen->editor->selected_type = 1;

                    break;
                }
            }
        }else{
            for (int i = 0; i < screen->sprite_nombre; i++)
            {

                sfIntRect cadre_screen = {sfSprite_GetX(screen->sprite[i]), sfSprite_GetY(screen->sprite[i]), sfSprite_GetX(screen->sprite[i])+sfSprite_GetWidth(screen->sprite[i]), sfSprite_GetY(screen->sprite[i])+sfSprite_GetHeight(screen->sprite[i]),};
                if (sfIntRect_Contains(&cadre_screen, mouse_x, mouse_y)){

                    sprite_Destroy(screen->editor->selected_sprite);
                    screen->editor->selected_sprite = sprite_Create(mouse_x, mouse_y, image_Get(screen->image, i), NULL);

                    screen->editor->selected_id = i;
                    screen->editor->selected_type = 0;

                    break;
                }
            }
        }
    }
}

void object_screen_Add_Animation(Object_Screen* screen, Animation* animation)
{

    Animation* temp_anim = animation_Create(sprite_GetImage(screen->editor->selected_sprite), animation->x, animation->y, animation->image_hauteur, animation->image_largeur, animation->nombre_image, 0, BOUCLE, animation->fps);
    sprite_Destroy(screen->editor->selected_sprite);
    screen->editor->selected_sprite = sprite_Create(0, 0, NULL, temp_anim);
    screen->editor->selected_type = 1;

    screen->animation_nombre++;
    assert(screen->animation = realloc(screen->animation, screen->animation_nombre*sizeof(Animation*)));

    screen->animation[screen->animation_nombre-1] = animation_Create(sprite_GetImage(screen->editor->selected_sprite), animation->x, animation->y, animation->image_hauteur, animation->image_largeur, animation->nombre_image, 0, BOUCLE, animation->fps);

    screen->x_cur = screen->espace+screen->x;
    screen->y_cur = screen->espace+screen->y;

    screen->y_max = 0;

    for (int i = 0; i < screen->animation_nombre; i++)
    {

        if (screen->x_cur+screen->animation[i]->image_largeur > screen->largeur)
        {
            screen->x_cur = screen->x+screen->espace;
            screen->y_cur = screen->espace+screen->y_max;
        }

        animation_SetPosition(screen->animation[i], screen->x_cur, screen->y_cur);

        screen->y_max = (screen->y_cur+screen->animation[i]->image_hauteur > screen->y_max) ? screen->y_cur+screen->animation[i]->image_hauteur : screen->y_max;
        screen->x_cur += screen->espace+screen->animation[i]->image_largeur;
    }

    widget_slide_SetNbrVal(screen->gui->widget_slide[1], (screen->y_max-screen->y)/(screen->hauteur-(2*screen->espace))+1);

}

void object_screen_Switch(Object_Screen* screen, void* rien)
{

    screen->type_affichage = !screen->type_affichage;

}
