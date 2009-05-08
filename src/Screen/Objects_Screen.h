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

#ifndef OBJECTSSCREEN_H
#define OBJECTSSCREEN_H

#include "SFML/Graphics.h"
#include "Gui/Gui.h"
#include "Objects/GameObjects.h"
#include "Objects/Editor.h"

typedef struct OBJECT_SCREEN
{

    sfRenderWindow* Game;

    sfSprite* background;

    Image* image;

    sfSprite** sprite;
    int sprite_nombre;

    Animation** animation;
    int animation_nombre;

    int type_affichage; //0 = image, 1 = animation

    int x;
    int y;
    int largeur;
    int hauteur;

    int x_cur;
    int y_cur;
    int y_max;
    int espace;

    int y_pos;

    int valeur;

    Gui* gui;

    Editor* editor;


}Object_Screen;

Object_Screen* object_screen_Create(sfRenderWindow*, sfImage*, int, int, int, int, Image*, Editor*);
void object_screen_Destroy(Object_Screen*);
void object_screen_Draw(Object_Screen*);
void object_screen_Load_Object(Object_Screen*, Image*); // Load sans copie et malloc, il faut donc un tableau d'object qui existe
void object_screen_Load_Animation(Object_Screen*, Animation**, int); // Load sans copie et malloc, il faut donc un tableau d'object qui existe (mais avec destruction)
void object_screen_Click(Object_Screen*, int, int);

void object_screen_Add_Animation(Object_Screen*, Animation*);
void object_screen_Switch(Object_Screen*, void*);

#endif
