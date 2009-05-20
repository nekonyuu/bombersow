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

#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "Objects/GameObjects.h"

typedef enum PHYSICS_TYPE {OBJECT, PLAYER, BULLET} Physics_type;

typedef enum QUAD_POS {NW, NE, SW, SE} Quad_pos;

typedef struct LIST_ELEMENT{

    struct LIST_ELEMENT* previous;
    void* elt;
    struct LIST_ELEMENT* next;

}List_element;

List_element* list_element_Create();
void list_element_Destroy(List_element*);
void list_element_Destroy_All(List_element*);
void list_element_Print(List_element*);

typedef struct LIST
{

    int taille;
    List_element* first;
    List_element* last;

    List_element* pointeur;

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

    int max_object;
    int max_depth;
    int depth;

    struct QUAD_TREE* noeuds[4];
    struct QUAD_TREE* parent;
    struct QUAD_TREE* first;

} QuadTree;

QuadTree* quadtree_Create();
void quadtree_Destroy(QuadTree*);
void quadtree_Generate(QuadTree*, Map*);
void quadtree_Add(QuadTree*, void*, int);
void quadtree_Delete_Node(QuadTree*);
void quadtree_Check_Node(QuadTree*, bool*);
void quadtree_Delete_Elt(void*, int);
void quadtree_Update(void*, int);
void quadtree_Print(QuadTree*);
void quadtree_Draw(sfRenderWindow*, QuadTree*);

bool IntRect_Contains(sfIntRect*, sfIntRect*);

#endif
