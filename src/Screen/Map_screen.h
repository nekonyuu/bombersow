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

#ifndef MAPSCREEN_H
#define MAPSCREEN_H

#include "SFML/Graphics.h"
#include "Gui/Gui.h"
#include "Objects/Editor.h"
#include "Objects/GameObjects.h"

typedef struct MAP_SCREEN
{

    sfRenderWindow* Game;

    sfSprite* background;

    Object*** object;
    int nombre_object[3];

    int x;
    int y;
    int largeur;
    int hauteur;

    Editor* editor;

}Map_screen;

Map_screen* map_screen_Create(sfRenderWindow*, sfImage*, Editor*, int, int, int, int);
void map_screen_Destroy(Map_screen*);
void map_screen_Draw(Map_screen*);
void map_screen_DrawPlan(Map_screen*, int);

void map_screen_Click(Map_screen*, int, int);

void map_screen_AddObj(Map_screen*, int, Object*);

#endif
