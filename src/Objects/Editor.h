#ifndef EDITOR_H
#define EDITOR_H

#include <stdbool.h>
#include "GraphicEngine/Image.h"
#include "Objects/GameObjects.h"

typedef struct EDITOR
{

    int mouse_x;
    int mouse_y;

    Animation* selected_animation;
    sfSprite* selected_image;
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
