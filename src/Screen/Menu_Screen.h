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

#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <SFML/Graphics.h>
#include "Gui/Gui.h"
#include "Objects/Editor.h"
#include "Objects/GameObjects.h"

#include "Screen/Objects_Screen.h"

typedef enum MENU_IMAGE {MENU_BACKGROUND, MENU_TEXTBOX_BACKGROUND, MENU_BOUTON}Menu_image;
typedef struct OBJECT_MENU
{

    sfRenderWindow* Game;

    sfSprite* background;

    Image* image;

    int x;
    int y;
    int largeur;
    int hauteur;

    Editor* editor;

    Gui* gui;

    Object_Screen* object_screen;

}Object_Menu;

Object_Menu* menu_screen_Create(sfRenderWindow*, Image*, int, int, int, int, Editor*, Object_Screen*);
void menu_screen_Destroy(Object_Menu* menu);
void menu_screen_Draw(Object_Menu* menu);
void menu_screen_Click(Object_Menu* menu, int mouse_x, int mouse_y);
void menu_screen_MouseOver(Object_Menu* menu, int mouse_x, int mouse_y);

#endif
