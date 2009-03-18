#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SFML/Graphics.h"
#include "GraphicEngine/Image.h"
#include "Objects/Editor.h"


Editor* editor_Create()
{

    Editor* editor = NULL;
    assert(editor = (Editor*)malloc(sizeof(Editor)));

    editor->selected_animation = NULL;
    editor->selected_image = NULL;
    editor->selected_type = 0;
    editor->selected_id = -1;

    editor->current_plan = 1;

    editor->animation_create = animation_Create(NULL, 0, 0, 0, 0, 0, 0, 0, 0);

    editor->nombre_object = 200;
    assert(editor->object = (Object**) malloc(editor->nombre_object*sizeof(Object)));
    for(int i = 0; i < editor->nombre_object; i++)
        editor->object[i] = NULL;

    editor->object_create = object_Create();


    return editor;

}

void editor_Destroy(Editor* editor)
{

    animation_Destroy(editor->animation_create);
    animation_Destroy(editor->selected_animation);

    for(int i = 0; i < editor->nombre_object; i++)
        object_Destroy(editor->object[i]);
    editor->object = NULL;

    object_Destroy(editor->object_create);



    free(editor);
    editor = NULL;

}

void editor_Draw(sfRenderWindow* Game, Editor* editor)
{

    if(editor->selected_type == 0)
        sfRenderWindow_DrawSprite(Game, editor->selected_image);
    else if(editor->selected_type == 1)
        animation_Draw(editor->selected_animation, Game);

}

void editor_MouseMove(Editor* editor, int x, int y)
{

    if(editor->selected_type == 0){
        if(sfSprite_GetWidth(editor->selected_image)+x < 1100){
            sfSprite_SetPosition(editor->selected_image, x, y);
        }else{
            sfSprite_SetPosition(editor->selected_image, 1100-sfSprite_GetWidth(editor->selected_image), y);
        }

        if(x > 200){
            sfSprite_SetPosition(editor->selected_image, sfSprite_GetX(editor->selected_image), y);
        }else{
            sfSprite_SetPosition(editor->selected_image, 200, y);
        }

        if(sfSprite_GetHeight(editor->selected_image)+y < 600){
            sfSprite_SetPosition(editor->selected_image, sfSprite_GetX(editor->selected_image), y);
        }else{
            sfSprite_SetPosition(editor->selected_image, sfSprite_GetX(editor->selected_image), 600-sfSprite_GetHeight(editor->selected_image));
        }
    }else{
        animation_SetPosition(editor->selected_animation, x, y);
    }

}
