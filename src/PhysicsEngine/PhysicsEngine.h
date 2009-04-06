#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "Objects/GameObjects.h"

typedef enum PHYSICS_TYPE {OBJECT, PLAYER, BULLET}Physics_type;

typedef enum QUAD_POS {NW, NE, SW, SE} Quad_pos;

typedef struct LIST{

    struct LIST* previous;
    void* elt;
    struct LIST* next;

}List;

List* list_Create();
void list_Add(List*, void*);
void list_Delete(List*);
void list_Destroy(List*);

typedef struct QUAD_TREE {

    List* bullet;
    List* object;
    List* player;

    sfIntRect rect;

    struct QUAD_TREE* noeuds[4];
    struct QUAD_TREE* parent;

}Quad_tree;

Quad_tree* quad_tree_Create();
void quad_tree_Destroy(Quad_tree*);
void quad_tree_Generate(Quad_tree*, Map*);
void quad_tree_Add(Quad_tree*, void*, int);
void quad_tree_Print(Quad_tree*);
void quad_tree_Draw(sfRenderWindow*, Quad_tree*);


typedef struct PHYSICAL_WORLD {

    Quad_tree* quad;

    float gravity; //en m.s^-2

}Physical_world;

#endif
