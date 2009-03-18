#ifndef EDITOR_H
#define EDITOR_H

#include <SFML/Graphics.h>
#include "GraphicEngine/Image.h"

typedef struct EDITOR
{
    int mouse_x;
    int mouse_y;

    Animation* selected_animation;
    sfSprite* selected_image;
    int selected_type;
    int selected_id;

    Animation* animation_create;

} Editor;

Editor* editor_Create();
void editor_Destroy(Editor*);
void editor_Draw(sfRenderWindow*, Editor*);
void editor_MouseMove(Editor*, int, int);

#endif
