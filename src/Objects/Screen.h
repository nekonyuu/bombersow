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

#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <Gui/Gui.h>

typedef enum { OPT_FONT, GUI_FONT, ALT_GUI_FONT } ScreenFontType;

typedef struct SCREEN
{
    sfImage** base_images;      // Images sources
    unsigned int nb_img;        // Nombre d'images source
    sfSprite** sprites;         // Sprites de l'écran
    unsigned int nb_spr;        // Nombre de sprites

    sfString** texts;           // Textes
    unsigned int nb_text;       // Nombre de textes
    sfFont* opt_font;           // Police des options
    // Intervalle contenant le menu
    unsigned int min_menu;
    unsigned int max_menu;

    Gui* gui;                   // Structure stockant les boutons et textbox
    sfFont* gui_font;           // Police de la GUI
    sfFont* alt_gui_font;       // Police alternative de la GUI

    sfMusic* music;             // Musique
} Screen;

Screen* screen_Create();
void screen_Destroy(Screen*);
void screen_LoadFont(Screen*, ScreenFontType, char*);
void screen_LoadText(Screen*, char*, sfColor, int, sfStringStyle, float, float);
void screen_SetMenuInterval(Screen*, unsigned int, unsigned int);
void screen_LoadMusic(Screen*, char*, sfBool);
void screen_PlayMusic(Screen*);
void screen_StopMusic(Screen*);
void screen_LoadImage(Screen*, char*);
void screen_AddTextbox(Screen*, int, int, int, int, int, sfImage*, sfColor, Widget_textbox_type, void*, sfColor, char*, sfColor, int);
void screen_Draw(Screen*, sfRenderWindow*);

#endif
