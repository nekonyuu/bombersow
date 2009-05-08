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

#ifndef EDITOR_H
#define EDITOR_H

#include <stdbool.h>
#include "GraphicEngine/Image.h"
#include "Objects/GameObjects.h"

typedef struct EDITOR
{

    int mouse_x;
    int mouse_y;

    Sprite* selected_sprite;
    _Bool selected_type;
    int selected_id;

    int bool_dynamic;
    int dynamic_step;

    int current_plan;

    Animation* animation_create;


    Object** object;
    int nombre_object;

    Object* object_create;
    Object* object_create_dynamic;

} Editor;

Editor* editor_Create();
void editor_Destroy(Editor*);
void editor_Draw(sfRenderWindow*, Editor*);
void editor_MouseMove(Editor*, int, int);
void editor_Add_Animation(Editor*, Animation*);

#endif
