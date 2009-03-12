#ifndef GRAVITYSYSTEM_H
#define GRAVITYSYSTEM_H

#include "Objects/Map.h"

typedef struct VECTOR2D
{
    float x, y;
} Vector2D;

typedef enum DIRECTION {UP, DOWN, LEFT, RIGHT} Direction;

void gravitysystem_WorldUpdate(Map*);

#endif
