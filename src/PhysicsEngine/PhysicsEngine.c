#include <stdio.h>
#include <stdlib.h>
#include "BaseSystem/Logging.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "Objects/GameObjects.h"

List_element* list_element_Create()
{

    List_element* list = NULL;
    assert(list = (List_element*) malloc(sizeof(List_element)));

    list->previous = NULL;
    list->elt = NULL;
    list->next = NULL;

    return list;
}

void list_element_Destroy(List_element* list)
{
    free_secure(list);
}

void list_element_Print(List_element* list)
{
    if (list != NULL)
    {
        printf("%p=>", list->elt);
        list_element_Print(list->next);
    }
}


List* list_Create()
{

    List* list = NULL;
    assert(list = (List*)malloc(sizeof(List)));

    list->first = NULL;
    list->last = NULL;
    list->taille = 0;

    return list;

}

void list_Add(List* list, void* elt, int type)
{

    List_element* list_element = list_element_Create();
    list_element->elt = elt;
    list_element->next = list->first;
    list_element->previous = NULL;

    list->first = list_element;
    if (list->taille == 0)
        list->last = list_element;
    else
        list_element->next->previous = list_element;
    list->taille++;

    if (type == OBJECT)
    {
        Object* obj = elt;
        obj->list_node = list_element;
    }
    else if (type == PLAYER)
    {
        Player* obj = elt;
        obj->list_node = list_element;
    }
    else
    {
        Bullet* obj = elt;
        obj->list_node = list_element;
    }

}

void list_Delete(List* list, List_element* list_element)
{
    if (list != NULL && list_element != NULL)
    {
        if (list->first == list_element)
        {
            list->first = list_element->next;
        }

        if (list->last == list_element)
        {
            list->last = list_element->previous;
        }

        list->taille--;
        if (list_element->previous != NULL)
            list_element->previous->next = list_element->next;
        if (list_element->next != NULL)
            list_element->next->previous = list_element->previous;

        list_element_Destroy(list_element);
    }
}

// A vérifier
void list_Destroy(List* list)
{
    if (list != NULL)
    {
        list_element_Destroy(list->first);
        free_secure(list);
    }
}

void list_Print(List* list)
{
    if (list != NULL)
    {
        printf("DEBUT=>");
        list_element_Print(list->first);
        printf("FIN\n");
    }
}

Quad_tree* quad_tree_Create()
{

    Quad_tree* quad = NULL;
    assert(quad = (Quad_tree*)malloc(sizeof(Quad_tree)));

    quad->bullet = NULL;
    quad->object = NULL;
    quad->player = NULL;

    quad->noeuds[NW] = NULL;
    quad->noeuds[NE] = NULL;
    quad->noeuds[SW] = NULL;
    quad->noeuds[SE] = NULL;

    quad->parent = NULL;

    quad->first = NULL;

    sfIntRect rect = {0,0,0,0};
    quad->rect = rect;

    return quad;

}
void quad_tree_Destroy(Quad_tree* quad)
{
    if (quad != NULL)
    {
        for (int i = 0; i < 4; i++)
            quad_tree_Destroy(quad->noeuds[i]);

        list_Destroy(quad->object);
        list_Destroy(quad->bullet);
        list_Destroy(quad->player);
        free_secure(quad);
    }
}

void quad_tree_Add(Quad_tree* quad, void* obj_, int type)
{

    sfIntRect rect_obj;
    if (type == OBJECT)
    {
        Object* obj = obj_;
        rect_obj = sprite_GetRect(obj->sprite);
    }
    else if (type == PLAYER)
    {
        Player* obj = obj_;
        rect_obj = sprite_GetRect(obj->sprite);
    }
    else
    {
        Bullet* obj = obj_;
        rect_obj = sprite_GetRect(obj->draw_image);
    }

    if (quad->object == NULL)
        quad->object = list_Create();

    if (quad->player == NULL)
        quad->player = list_Create();

    if (quad->bullet == NULL)
        quad->bullet = list_Create();

    if ( sfIntRect_Intersects(&quad->rect, &rect_obj, NULL) )
    {

        if (quad->noeuds[NW] == NULL)
        {
            sfIntRect rectNW = {quad->rect.Left, quad->rect.Top, (quad->rect.Right+quad->rect.Left)/2, (quad->rect.Bottom+quad->rect.Top)/2};
            quad->noeuds[NW] = quad_tree_Create();
            quad->noeuds[NW]->rect = rectNW;
            quad->noeuds[NW]->parent = quad;
            quad->noeuds[NW]->first = quad->first;

            sfIntRect rectNE = {(quad->rect.Right+quad->rect.Left)/2, quad->rect.Top, quad->rect.Right, (quad->rect.Bottom+quad->rect.Top)/2};
            quad->noeuds[NE] = quad_tree_Create();
            quad->noeuds[NE]->rect = rectNE;
            quad->noeuds[NE]->parent = quad;
            quad->noeuds[NE]->first = quad->first;

            sfIntRect rectSW = {quad->rect.Left, (quad->rect.Bottom+quad->rect.Top)/2, (quad->rect.Right+quad->rect.Left)/2, quad->rect.Bottom};
            quad->noeuds[SW] = quad_tree_Create();
            quad->noeuds[SW]->rect = rectSW;
            quad->noeuds[SW]->parent = quad;
            quad->noeuds[SW]->first = quad->first;

            sfIntRect rectSE = {(quad->rect.Right+quad->rect.Left)/2, (quad->rect.Bottom+quad->rect.Top)/2, quad->rect.Right, quad->rect.Bottom};
            quad->noeuds[SE] = quad_tree_Create();
            quad->noeuds[SE]->rect = rectSE;
            quad->noeuds[SE]->parent = quad;
            quad->noeuds[SE]->first = quad->first;
        }

        int nombre_noeuds = 0;
        for (int i = 0; i < 4; i++)
        {
            if ( sfIntRect_Intersects(&quad->noeuds[i]->rect, &rect_obj, NULL) )
            {
                nombre_noeuds++;
            }
        }

        if (nombre_noeuds == 1)
        {
            for (int i = 0; i < 4; i++)
                quad_tree_Add(quad->noeuds[i], obj_, type);
        }
        else
        {

            if (type == OBJECT)
            {
                Object* obj = obj_;
                list_Add(quad->object, obj, type);
                obj->quad_node = quad;
            }
            else if (type == PLAYER)
            {
                Player* obj = obj_;
                list_Add(quad->player, obj, type);
                obj->quad_node = quad;
            }
            else
            {
                Bullet* obj = obj_;
                list_Add(quad->bullet, obj, type);
                obj->quad_node = quad;
            }
        }
    }
}

void quad_tree_Print(Quad_tree* quad)
{
    if (quad != NULL)
    {
        printf("%d %d %d %d\n", quad->rect.Left, quad->rect.Top, quad->rect.Right, quad->rect.Bottom);
        for (int i = 0; i < 4; i++)
            quad_tree_Print(quad->noeuds[i]);
    }
}

void quad_tree_Draw(sfRenderWindow* Game, Quad_tree* quad)
{
    if (quad != NULL)
    {
        sfShape* test = NULL;
        if (quad->player != NULL && quad->player->taille > 0)
        {
            test = sfShape_CreateRectangle(quad->rect.Left+1, quad->rect.Top+1, quad->rect.Right-1, quad->rect.Bottom-1, sfColor_FromRGBA(255,255,255,0), 1, sfRed);
        }
        else
        {
            test = sfShape_CreateRectangle(quad->rect.Left+1, quad->rect.Top+1, quad->rect.Right-1, quad->rect.Bottom-1, sfColor_FromRGBA(255,255,255,0), 1, sfWhite);
        }
        sfRenderWindow_DrawShape(Game, test);
        sfShape_Destroy(test);
        for (int i = 0; i < 4; i++)
            quad_tree_Draw(Game, quad->noeuds[i]);
    }
}

void quad_tree_Generate(Quad_tree* quad, Map* map)
{

    for (int i = 0; i < map->nb_objects; i++)
    {
        quad_tree_Add(quad, map->objects_list[i], OBJECT);
    }

    for (int i = 0; i < map->nb_players; i++)
    {
        quad_tree_Add(quad, map->players_list[i], PLAYER);
    }

    for (int i = 0; i < map->nb_bullets; i++)
    {
        quad_tree_Add(quad, map->bullets_list[i], BULLET);
    }
}

void quad_tree_Delete_Elt(void* obj_, int type)
{
    if (type == OBJECT)
    {
        Object* obj = obj_;
        list_Delete(obj->quad_node->object, obj->list_node);
        quad_tree_Delete_Node(obj->quad_node);
    }
    else if (type == PLAYER)
    {
        Player* obj = obj_;
        list_Delete(obj->quad_node->player, obj->list_node);
        quad_tree_Delete_Node(obj->quad_node);
    }
    else
    {
        Bullet* obj = obj_;
        list_Delete(obj->quad_node->bullet, obj->list_node);
        quad_tree_Delete_Node(obj->quad_node);
    }

}

void quad_tree_Check_Node(Quad_tree* quad, _Bool* check)
{

    if (*check == 0)
        return;

    if (quad != NULL && quad->bullet != NULL && quad->player != NULL && quad->object != NULL)
    {
        if (quad->object->taille == 0 && quad->player->taille == 0 && quad->bullet->taille == 0)
        {
            for (int i = 0; i < 4; i++)
                quad_tree_Check_Node(quad->noeuds[i], check);
        }
        else
        {
            *check = 0;
        }
    }
}

void quad_tree_Delete_Node(Quad_tree* quad)
{

    _Bool test;
    test = 1;
    quad_tree_Check_Node(quad, &test);
    if (test)
    {
        for (int i = 0; i < 4; i++)
        {
            quad_tree_Destroy(quad->noeuds[i]);
            quad->noeuds[i] = NULL;
        }
        quad_tree_Delete_Node(quad->parent);
    }

}

void quad_tree_Update(void* obj_, int type)
{

    sfIntRect rect_obj;
    Quad_tree* quad = NULL;
    if (type == OBJECT)
    {
        Object* obj = obj_;
        quad = obj->quad_node;
        rect_obj = sprite_GetRect(obj->sprite);
    }
    else if (type == PLAYER)
    {
        Player* obj = obj_;
        quad = obj->quad_node;
        rect_obj = sprite_GetRect(obj->sprite);
    }
    else
    {
        Bullet* obj = obj_;
        quad = obj->quad_node;
        rect_obj = sprite_GetRect(obj->draw_image);
    }

    if (!IntRect_Contains(&quad->rect, &rect_obj))
    {
        quad_tree_Delete_Elt(obj_, type);
        quad_tree_Add(quad->first, obj_, type);
    }

}

_Bool IntRect_Contains(sfIntRect* rect, sfIntRect* rect2)
{

    if (sfIntRect_Intersects(rect, rect2, NULL))
        if (rect->Top >= rect2->Top && rect->Bottom <= rect2->Bottom && rect->Left >= rect2->Left && rect->Right <= rect2->Right)
            return true;
        else
            return false;
    else
        return false;

}
