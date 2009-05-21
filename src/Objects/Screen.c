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

#include "BaseSystem/Logging.h"
#include "Objects/Screen.h"

// Constructeur
Screen* screen_Create()
{
    Screen* new_screen = NULL;

    assert(new_screen = (Screen*) malloc(sizeof(Screen)));

    new_screen->base_images = NULL;
    new_screen->nb_img = 0;
    new_screen->sprites = NULL;
    new_screen->nb_spr = 0;

    new_screen->texts = NULL;
    new_screen->nb_text = 0;
    new_screen->opt_font = NULL;
    new_screen->min_menu = 0;
    new_screen->max_menu = 0;

    new_screen->gui = gui_Create();
    new_screen->gui_font = NULL;
    new_screen->alt_gui_font = NULL;

    new_screen->music = NULL;

    return new_screen;
}

// Destructeur
void screen_Destroy(Screen* screen2destroy)
{
    assert(screen2destroy);

    for (int i = 0; i < screen2destroy->nb_text; i++)
        sfString_Destroy(screen2destroy->texts[i]);
    free_secure(screen2destroy->texts);

    for (int i = 0; i < screen2destroy->nb_spr; i++)
        sfSprite_Destroy(screen2destroy->sprites[i]);
    free_secure(screen2destroy->sprites);

    for (int i = 0; i < screen2destroy->nb_img; i++)
        sfImage_Destroy(screen2destroy->base_images[i]);
    free_secure(screen2destroy->base_images);

    sfMusic_Destroy(screen2destroy->music);
    gui_Destroy(screen2destroy->gui);

    sfFont_Destroy(screen2destroy->opt_font);
    sfFont_Destroy(screen2destroy->gui_font);
    sfFont_Destroy(screen2destroy->alt_gui_font);

    free_secure(screen2destroy);
}

// Charge une police
void screen_LoadFont(Screen* screen, ScreenFontType type, char* path)
{
    if (!path)
        logging_Error("screen_LoadFont", "No path sent", NULL_PTR);

    switch(type)
    {
    case OPT_FONT:
        screen->opt_font = sfFont_CreateFromFile(path, 50, NULL);
        break;
    case GUI_FONT:
        screen->gui_font = sfFont_CreateFromFile(path, 50, NULL);
        break;
    case ALT_GUI_FONT:
        screen->alt_gui_font = sfFont_CreateFromFile(path, 50, NULL);
        break;
    default:
        break;
    }
    return;
}

// Ajout d'une sfString dans Screen
void screen_LoadText(Screen* screen, char* text, sfColor color, int font_size, sfStringStyle style, float x, float y)
{
    if (!screen)
        logging_Error("screen_LoadText", "Screen object sent NULL", NULL_PTR);

    assert(screen->texts = (sfString**) realloc(screen->texts, ++screen->nb_text * sizeof(sfString*)));

    screen->texts[screen->nb_text - 1] = sfString_Create();
    sfString_SetText(screen->texts[screen->nb_text - 1], text);
    sfString_SetFont(screen->texts[screen->nb_text - 1], screen->opt_font);
    sfString_SetColor(screen->texts[screen->nb_text - 1], color);
    sfString_SetStyle(screen->texts[screen->nb_text - 1], style);
    sfString_SetSize(screen->texts[screen->nb_text - 1], font_size);
    sfString_SetPosition(screen->texts[screen->nb_text - 1], x, y);
}

void screen_HightlightText(Screen* screen, unsigned int id, sfColor color)
{
    for(int i = 1; i < screen->nb_text; i++)
    {
        if(i == id)
            sfString_SetColor(screen->texts[i], color);
        else
            sfString_SetColor(screen->texts[i], sfWhite);
    }
}

void screen_SetMenuInterval(Screen* screen, unsigned int min, unsigned int max)
{
    screen->min_menu = min;
    screen->max_menu = max;
}

// Charge une musique dans un Screen
void screen_LoadMusic(Screen* screen, char* path, sfBool loop)
{
    if (!path)
        logging_Error("screen_LoadMusic", "No music path sent", NULL_PTR);

    screen->music = sfMusic_CreateFromFile(path);
    sfMusic_SetLoop(screen->music, loop);
}

// Lecture de la musique de l'écran donné
void screen_PlayMusic(Screen* screen)
{
    if (screen->music)
        sfMusic_Play(screen->music);
}

// Arrêt de la musique
void screen_StopMusic(Screen* screen)
{
    if (screen->music)
        sfMusic_Stop(screen->music);
}

// Charge une image dans un screen
void screen_LoadImage(Screen* screen, char* path)
{
    if (!path)
        logging_Error("screen_LoadImage", "No image path sent", NULL_PTR);
    if (!screen)
        logging_Error("screen_LoadImage", "Screen object sent NULL", NULL_PTR);

    assert(screen->base_images = (sfImage**) realloc(screen->base_images, ++screen->nb_img * sizeof(sfImage*)));

    assert(screen->sprites = (sfSprite**) realloc(screen->sprites, ++screen->nb_spr * sizeof(sfSprite*)));

    screen->base_images[screen->nb_img - 1] = sfImage_CreateFromFile(path);
    screen->sprites[screen->nb_spr - 1] = sfSprite_Create();
    sfSprite_SetImage(screen->sprites[screen->nb_img - 1], screen->base_images[screen->nb_img - 1]);
}

// Ajout d'une textbox à l'écran
void screen_AddTextbox(Screen* screen, int x, int y, int width, int height, int length, sfImage* image,
                       sfColor border_color, Widget_textbox_type type, void* var, sfColor text_color, char* text,
                       sfColor label_color, int text_size)
{
    if(!screen)
        logging_Error("screen_AddTextbox", "Screen object sent NULL", NULL_PTR);

    Widget_textbox* textbox = widget_textbox_Create(x, y, width, height, length, image, border_color, type, var,
        text_color, text, label_color, (type == INT) ? screen->alt_gui_font : screen->gui_font, text_size);

    gui_Add_Textbox(screen->gui, textbox);
}

Gui* screen_GetGUI(Screen* ptr)
{
    return ptr->gui;
}

// Met le focus sur la textbox
void screen_SetActiveTextbox(Screen* screen, int id)
{
    screen->gui->widget_textbox[id]->active = true;
}

// Supprime le focus sur la textbox
void screen_SetInactiveTextbox(Screen* screen, int id)
{
    screen->gui->widget_textbox[id]->active = false;
}

// Dessin des objets de l'écran
void screen_Draw(Screen* screen, sfRenderWindow* Game)
{
    if (!screen)
        logging_Error("screen_DrawText", "Screen object sent NULL", NULL_PTR);

    // Dessin images
    for(int i = 0; i < screen->nb_spr; i++)
        sfRenderWindow_DrawSprite(Game, screen->sprites[i]);

    // Dessin textes
    for (int i = 0; i < screen->nb_text; i++)
        sfRenderWindow_DrawString(Game, screen->texts[i]);

    // Dessin GUI
    gui_Draw(Game, screen->gui);
}
