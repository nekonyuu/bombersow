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

    new_screen->images = NULL;
    new_screen->nb_img = 0;

    new_screen->texts = NULL;
    new_screen->nb_text = 0;

    new_screen->gui = gui_Create();

    new_screen->font = NULL;
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

    for (int i = 0; i < screen2destroy->nb_img; i++)
        sfSprite_Destroy(screen2destroy->images[i]);
    free_secure(screen2destroy->images);

    screen2destroy->font = NULL;
    sfMusic_Destroy(screen2destroy->music);
    gui_Destroy(screen2destroy->gui);
    free_secure(screen2destroy);
}

// Charge une police
void screen_LoadFont(Screen* screen, sfFont* font_)
{
    if (!font_)
        logging_Error("screen_LoadFont", "sfFont object sent NULL");

    screen->font = font_;
}

// Ajout d'une sfString dans Screen
void screen_LoadText(Screen* screen, char* text, sfColor color, int font_size, sfStringStyle style, float x, float y)
{
    if (!screen)
        logging_Error("screen_LoadText", "Screen object sent NULL");

    screen->nb_text++;
    if (!screen->texts)
        assert(screen->texts = (sfString**) malloc(sizeof(sfString*)));
    else
        assert(screen->texts = (sfString**) realloc(screen->texts, screen->nb_text * sizeof(sfString*)));

    screen->texts[screen->nb_text - 1] = sfString_Create();
    sfString_SetText(screen->texts[screen->nb_text - 1], text);
    sfString_SetFont(screen->texts[screen->nb_text - 1], screen->font);
    sfString_SetColor(screen->texts[screen->nb_text - 1], color);
    sfString_SetStyle(screen->texts[screen->nb_text - 1], style);
    sfString_SetSize(screen->texts[screen->nb_text - 1], font_size);
    sfString_SetPosition(screen->texts[screen->nb_text - 1], x, y);
}

// Dessin de texte
void screen_DrawText(sfRenderWindow* window, Screen* screen, int id)
{
    if (!screen)
        logging_Error("screen_DrawText", "Screen object sent NULL");
    if (!screen->texts)
        logging_Warning("screen_DrawText", "No texts loaded in Screen object sent");
    else
        sfRenderWindow_DrawString(window, screen->texts[id]);
}

// Charge une musique dans un Screen
void screen_LoadMusic(Screen* screen, sfMusic* music, sfBool loop)
{
    if (!music)
        logging_Warning("screen_LoadMusic", "sfMusic object sent NULL");

    screen->music = music;
    sfMusic_SetLoop(screen->music, loop);
}

// Lecture de la musique de l'écran donné
void screen_PlayMusic(Screen* screen)
{
    if (screen->music)
        sfMusic_Play(screen->music);
}

// Charge un arrière plan dans un Screen
void screen_LoadImage(Screen* screen, sfImage* image)
{
    if (!image)
        logging_Error("screen_LoadImage", "sfImage object sent NULL");
    if (!screen)
        logging_Error("screen_LoadImage", "Screen object sent NULL");

    screen->nb_img++;
    if (!screen->images)
        assert(screen->images = (sfSprite**) malloc(sizeof(sfSprite*)));
    else
        assert(screen->images = (sfSprite**) realloc(screen->images, screen->nb_img * sizeof(sfSprite*)));

    screen->images[screen->nb_img - 1] = sfSprite_Create();
    sfSprite_SetImage(screen->images[screen->nb_img - 1], image);
}

void screen_AddTextbox(Screen* screen, int x, int y, int width, int height, int length, sfImage* image,
                       sfColor border_color, Widget_textbox_type type, void* var, char* text,
                       sfColor label_color, sfFont* label_font, int text_size)
{
    if(!screen)
        logging_Error("screen_AddTextbox", "Screen object sent NULL");
    Widget_textbox* textbox = widget_textbox_Create(x, y, width, height, length, image, border_color, type, var, text, label_color, label_font, text_size);
    gui_Add_Textbox(screen->gui, textbox);
}

void screen_DrawGui(sfRenderWindow* Game, Screen* screen)
{
    gui_Draw(Game, screen->gui);
}
