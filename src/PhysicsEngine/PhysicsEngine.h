#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "Objects/GameObjects.h"

typedef enum PHYSICS_TYPE {OBJECT, PLAYER, BULLET}Physics_type;

typedef enum QUAD_POS {NW, NE, SW, SE} Quad_pos;

typedef struct LIST_ELEMENT{

    struct LIST_ELEMENT* previous;
    void* elt;
    struct LIST_ELEMENT* next;

}List_element;

List_element* list_element_Create();
void list_element_Destroy(List_element*);
void list_element_Print(List_element*);

typedef struct LIST
{

    int taille;
    List_element* first;
    List_element* last;

}List;

List* list_Create();
void list_Add(List*, void*, int);
void list_Delete(List*, List_element*);
void list_Destroy(List*);
void list_Print(List*);

typedef struct QUAD_TREE {

    List* bullet;
    List* object;
    List* player;

    sfIntRect rect;

    struct QUAD_TREE* noeuds[4];
    struct QUAD_TREE* parent;
    struct QUAD_TREE* first;

}Quad_tree;

Quad_tree* quad_tree_Create();
void quad_tree_Destroy(Quad_tree*);
void quad_tree_Generate(Quad_tree*, Map*);
void quad_tree_Add(Quad_tree*, void*, int);
void quad_tree_Delete_Node(Quad_tree*);
void quad_tree_Check_Node(Quad_tree*, _Bool*);
void quad_tree_Delete_Elt(void*, int);
void quad_tree_Update(void*, int);
void quad_tree_Print(Quad_tree*);
void quad_tree_Draw(sfRenderWindow*, Quad_tree*);

#endif
