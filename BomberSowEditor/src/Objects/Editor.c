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

    editor->animation_create = animation_Create(NULL, 0, 0, 0, 0, 0, 0, 0, 0);

    return editor;

}
void editor_Destroy(Editor* editor)
{

    animation_Destroy(editor->animation_create);
    animation_Destroy(editor->selected_animation);

    free(editor);
    editor = NULL;

}

void editor_Draw(sfRenderWindow* Game, Editor* editor)
{

    if(editor->selected_type == 1)
        sfRenderWindow_DrawSprite(Game, editor->selected_image);
    else if(editor->selected_type == 2)
        animation_Draw(editor->selected_animation, Game);

}

void editor_MouseMove(Editor* editor, int x, int y)
{

    if(editor->selected_type == 1)
        sfSprite_SetPosition(editor->selected_image, x, y);
    else
        animation_SetPosition(editor->selected_animation, x, y);

}
