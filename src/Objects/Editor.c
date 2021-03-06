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
    This work is licensed under the Creative Commons Attribution-Share Alike 3.0 Unported License.
    To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/
    or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SFML/Graphics.h"
#include "GraphicEngine/Image.h"
#include "Objects/GameObjects.h"
#include "Objects/Editor.h"


Editor* editor_Create()
{

    Editor* editor = NULL;
    assert(editor = (Editor*)malloc(sizeof(Editor)));

    editor->selected_sprite = NULL;
    editor->selected_type = 0;
    editor->selected_id = -1;

    editor->current_plan = 1;

    editor->bool_dynamic = 0;
    editor->dynamic_step = 0;

    editor->animation_create = animation_Create(NULL, 0, 0, 0, 0, 0, 0, 0, 0);

    editor->nombre_object = 200;
    assert(editor->object = (Object**) malloc(editor->nombre_object*sizeof(Object)));
    for(int i = 0; i < editor->nombre_object; i++)
        editor->object[i] = NULL;

    editor->object_create = object_Create(0, PLATFORM, 0, 0, NULL, NULL);

    editor->object_create_dynamic = NULL;


    return editor;

}

void editor_Destroy(Editor* editor)
{

    animation_Destroy(editor->animation_create);

    sprite_Destroy(editor->selected_sprite);
    puts("CONNARD 1");
    for(int i = 0; i < editor->nombre_object; i++)
        object_Destroy(editor->object[i]);

    editor->object = NULL;
    object_Destroy(editor->object_create);
    object_Destroy(editor->object_create_dynamic);


    free(editor);
    editor = NULL;

}

void editor_Draw(sfRenderWindow* Game, Editor* editor)
{

    if(editor->dynamic_step == 1)
        sprite_SetColor(editor->selected_sprite, sfColor_FromRGBA(255,255,255,120));
    else
        sprite_SetColor(editor->selected_sprite, sfColor_FromRGBA(255,255,255,255));

    sprite_Draw(Game, editor->selected_sprite);

}

void editor_MouseMove(Editor* editor, int x, int y)
{

    if(editor->selected_sprite->largeur+x < 1100){
        sprite_SetPosition(editor->selected_sprite, x, y);
    }else{
        sprite_SetPosition(editor->selected_sprite, 1100-editor->selected_sprite->largeur, y);
    }

    if(x > 200){
        sprite_SetPosition(editor->selected_sprite, editor->selected_sprite->x, y);
    }else{
        sprite_SetPosition(editor->selected_sprite, 200, y);
    }

    if(editor->selected_sprite->hauteur+y < 600){
        sprite_SetPosition(editor->selected_sprite, editor->selected_sprite->x, y);
    }else{
        sprite_SetPosition(editor->selected_sprite, editor->selected_sprite->x, 600-editor->selected_sprite->hauteur);
    }


}


void editor_Add_Animation(Editor* editor, Animation* animation){

    Animation* a_ = animation_Create(sprite_GetImage(editor->selected_sprite), animation->x, animation->y, animation->image_hauteur, animation->image_largeur, animation->nombre_image, 0, BOUCLE, animation->fps);
    sprite_Destroy(editor->selected_sprite);
    editor->selected_sprite = sprite_Create(0, 0, NULL, a_);
    editor->selected_type = 1;

}
