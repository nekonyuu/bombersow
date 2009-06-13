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

#include <stdio.h>
#include <stdlib.h>
#include "BaseSystem/Logging.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "Objects/GameObjects.h"

sfMutex* g_QuadTree_Access;

void PhysicsEngine_Init()
{
    g_QuadTree_Access = sfMutex_Create();
}

void PhysicsEngine_Clean()
{
    sfMutex_Destroy(g_QuadTree_Access);
}

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
    free(list);
}

void list_element_Destroy_All(List_element* list)
{
    if (list != NULL)
    {
        list_element_Destroy_All(list->next);
        free(list);
    }
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

    list->pointeur = NULL;

    return list;

}

int list_Next(List* list)
{
    list->pointeur = list->pointeur->next;
    if (list->pointeur == NULL)
        return 0;
    else
        return 1;
}

List_element* list_Get(List* list)
{
    return list->pointeur;
}

void list_Add(List* list, void* elt, int type)
{

    List_element* list_element = list_element_Create();
    list_element->elt = elt;
    list_element->next = list->first;
    list_element->previous = NULL;

    list->first = list_element;
    list->pointeur = list_element;

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
        list_element_Destroy_All(list->first);
        free(list);
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

/*
QuadTree* quadtree_Create()
{

    QuadTree* quad = NULL;
    assert(quad = (QuadTree*)malloc(sizeof(QuadTree)));

    quad->bullet = NULL;
    quad->object = NULL;
    quad->player = NULL;

    quad->noeuds[NW] = NULL;
    quad->noeuds[NE] = NULL;
    quad->noeuds[SW] = NULL;
    quad->noeuds[SE] = NULL;

    quad->max_object = 5;
    quad->max_depth = 10;
    quad->depth = 0;

    quad->parent = NULL;

    quad->first = NULL;

    sfIntRect rect = {0,0,0,0};
    quad->rect = rect;

    return quad;

}

void quadtree_Destroy(QuadTree* quad)
{
    if (quad != NULL)
    {
        list_Destroy(quad->object);
        list_Destroy(quad->bullet);
        list_Destroy(quad->player);

        for (int i = 0; i < 4; i++)
            quadtree_Destroy(quad->noeuds[i]);
        free(quad);
    }
}

void quadtree_Add_Node(QuadTree* quad, void* obj_, int type)
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

void quadtree_Create_Node(QuadTree* quad)
{
    sfIntRect rectNW = {quad->rect.Left, quad->rect.Top, (quad->rect.Right+quad->rect.Left)/2, (quad->rect.Bottom+quad->rect.Top)/2};
    quad->noeuds[NW] = quadtree_Create();
    quad->noeuds[NW]->rect = rectNW;
    quad->noeuds[NW]->parent = quad;
    quad->noeuds[NW]->first = quad->first;
    quad->noeuds[NW]->depth = quad->depth++;

    sfIntRect rectNE = {(quad->rect.Right+quad->rect.Left)/2, quad->rect.Top, quad->rect.Right, (quad->rect.Bottom+quad->rect.Top)/2};
    quad->noeuds[NE] = quadtree_Create();
    quad->noeuds[NE]->rect = rectNE;
    quad->noeuds[NE]->parent = quad;
    quad->noeuds[NE]->first = quad->first;
    quad->noeuds[NE]->depth = quad->depth++;

    sfIntRect rectSW = {quad->rect.Left, (quad->rect.Bottom+quad->rect.Top)/2, (quad->rect.Right+quad->rect.Left)/2, quad->rect.Bottom};
    quad->noeuds[SW] = quadtree_Create();
    quad->noeuds[SW]->rect = rectSW;
    quad->noeuds[SW]->parent = quad;
    quad->noeuds[SW]->first = quad->first;
    quad->noeuds[SW]->depth = quad->depth++;

    sfIntRect rectSE = {(quad->rect.Right+quad->rect.Left)/2, (quad->rect.Bottom+quad->rect.Top)/2, quad->rect.Right, quad->rect.Bottom};
    quad->noeuds[SE] = quadtree_Create();
    quad->noeuds[SE]->rect = rectSE;
    quad->noeuds[SE]->parent = quad;
    quad->noeuds[SE]->first = quad->first;
    quad->noeuds[SE]->depth = quad->depth++;
}

void quadtree_Add(QuadTree* quad, void* obj_, int type)
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
            if(quad->object->taille+quad->player->taille+quad->bullet->taille < quad->max_object)
            {
                quadtree_Add_Node(quad, obj_, type);
            }
            else
            {
                if(quad->depth < quad->max_depth)
                {
                    quadtree_Create_Node(quad);

                    do{
                        List_element* elt = list_Get(quad->player);
                        quadtree_Add(quad->noeuds[NW], elt->elt, PLAYER);
                        quadtree_Add(quad->noeuds[SW], elt->elt, PLAYER);
                        quadtree_Add(quad->noeuds[NE], elt->elt, PLAYER);
                        quadtree_Add(quad->noeuds[SE], elt->elt, PLAYER);
                    }while(list_Next(quad->player));
                    list_Destroy(quad->player);
                    quad->player = NULL;

                    quadtree_Add(quad->noeuds[NW], obj_, type);
                    quadtree_Add(quad->noeuds[SW], obj_, type);
                    quadtree_Add(quad->noeuds[NE], obj_, type);
                    quadtree_Add(quad->noeuds[SE], obj_, type);
                }
                else
                {
                    quadtree_Add_Node(quad, obj_, type);
                }
            }
        }
        else
        {
            quadtree_Add(quad->noeuds[NW], obj_, type);
            quadtree_Add(quad->noeuds[SW], obj_, type);
            quadtree_Add(quad->noeuds[NE], obj_, type);
            quadtree_Add(quad->noeuds[SE], obj_, type);
        }
    }
}

void quadtree_Print(QuadTree* quad)
{
    if (quad != NULL)
    {
        printf("%d %d %d %d\n", quad->rect.Left, quad->rect.Top, quad->rect.Right, quad->rect.Bottom);
        for (int i = 0; i < 4; i++)
            quadtree_Print(quad->noeuds[i]);
    }
}

void quadtree_Draw(sfRenderWindow* Game, QuadTree* quad)
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
            quadtree_Draw(Game, quad->noeuds[i]);
    }
}

void quadtree_Generate(QuadTree* quad, Map* map)
{

    for (int i = 0; i < map->nb_objects; i++)
    {
        quadtree_Add(quad, map->objects_list[i], OBJECT);
    }

    for (int i = 0; i < map->nb_players; i++)
    {
        quadtree_Add(quad, map->players_list[i], PLAYER);
    }

    for (int i = 0; i < map->nb_bullets; i++)
    {
        quadtree_Add(quad, map->bullets_list[i], BULLET);
    }
}

void quadtree_Delete_Elt(void* obj_, int type)
{
    if (type == OBJECT)
    {
        Object* obj = obj_;
        list_Delete(obj->quad_node->object, obj->list_node);
        quadtree_Delete_Node(obj->quad_node);
    }
    else if (type == PLAYER)
    {
        Player* obj = obj_;
        list_Delete(obj->quad_node->player, obj->list_node);
        quadtree_Delete_Node(obj->quad_node);
    }
    else
    {
        Bullet* obj = obj_;
        list_Delete(obj->quad_node->bullet, obj->list_node);
        quadtree_Delete_Node(obj->quad_node);
    }

}

void quadtree_Check_Node(QuadTree* quad, bool* check)
{

    if (*check == 0)
        return;

    if (quad != NULL && quad->bullet != NULL && quad->player != NULL && quad->object != NULL)
    {
        if (quad->object->taille == 0 && quad->player->taille == 0 && quad->bullet->taille == 0)
        {
            for (int i = 0; i < 4; i++)
                quadtree_Check_Node(quad->noeuds[i], check);
        }
        else
        {
            *check = 0;
        }
    }
}

void quadtree_Delete_Node(QuadTree* quad)
{

    bool test;
    test = 1;
    quadtree_Check_Node(quad, &test);
    if (test)
    {
        for (int i = 0; i < 4; i++)
        {
            quadtree_Destroy(quad->noeuds[i]);
            quad->noeuds[i] = NULL;
        }
        quadtree_Delete_Node(quad->parent);
    }

}


void quadtree_Update(void* obj_, int type)
{

    sfIntRect rect_obj;
    QuadTree* quad = NULL;
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
        quadtree_Delete_Elt(obj_, type);
        quadtree_Add(quad->first, obj_, type);
    }

}

int IntRect_Contains(sfIntRect* rect, sfIntRect* rect2)
{

    if (sfIntRect_Intersects(rect, rect2, NULL))
        if (rect->Top <= rect2->Top && rect->Bottom >= rect2->Bottom && rect->Left <= rect2->Left && rect->Right >= rect2->Right)
            return 1;
        else
            return 0;
    else
        return 0;

}
*/

QuadTree* quadtree_Create()
{

    QuadTree* quad = NULL;
    assert(quad = (QuadTree*)malloc(sizeof(QuadTree)));

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

void quadtree_Destroy(QuadTree* quad)
{
    if (quad != NULL)
    {
        list_Destroy(quad->object);
        list_Destroy(quad->bullet);
        list_Destroy(quad->player);

        for (int i = 0; i < 4; i++)
            quadtree_Destroy(quad->noeuds[i]);
        free(quad);
    }
}

void quadtree_Add(QuadTree* quad, void* obj_, int type)
{

    //Meme chose que dans collision_Detection
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

    sfMutex_Lock(g_QuadTree_Access);

    if (quad->object == NULL)
        quad->object = list_Create();

    if (quad->player == NULL)
        quad->player = list_Create();

    if (quad->bullet == NULL)
        quad->bullet = list_Create();

    //Determine si l'objet est dans le rect du Quad_tree
    if ( sfIntRect_Intersects(&quad->rect, &rect_obj, NULL) )
    {

        //Si les noeuds fils n'existe pas on les créé
        if (quad->noeuds[NW] == NULL)
        {
            sfIntRect rectNW = {quad->rect.Left, quad->rect.Top, (quad->rect.Right+quad->rect.Left)/2, (quad->rect.Bottom+quad->rect.Top)/2};
            quad->noeuds[NW] = quadtree_Create();
            quad->noeuds[NW]->rect = rectNW;
            quad->noeuds[NW]->parent = quad;
            quad->noeuds[NW]->first = quad->first;
        }

        if (quad->noeuds[NE] == NULL)
        {
            sfIntRect rectNE = {(quad->rect.Right+quad->rect.Left)/2+1, quad->rect.Top, quad->rect.Right, (quad->rect.Bottom+quad->rect.Top)/2};
            quad->noeuds[NE] = quadtree_Create();
            quad->noeuds[NE]->rect = rectNE;
            quad->noeuds[NE]->parent = quad;
            quad->noeuds[NE]->first = quad->first;
        }

        if (quad->noeuds[SW] == NULL)
        {
            sfIntRect rectSW = {quad->rect.Left, (quad->rect.Bottom+quad->rect.Top)/2+1, (quad->rect.Right+quad->rect.Left)/2, quad->rect.Bottom};
            quad->noeuds[SW] = quadtree_Create();
            quad->noeuds[SW]->rect = rectSW;
            quad->noeuds[SW]->parent = quad;
            quad->noeuds[SW]->first = quad->first;
        }

        if (quad->noeuds[SE] == NULL)
        {
            sfIntRect rectSE = {(quad->rect.Right+quad->rect.Left)/2+1, (quad->rect.Bottom+quad->rect.Top)/2+1, quad->rect.Right, quad->rect.Bottom};
            quad->noeuds[SE] = quadtree_Create();
            quad->noeuds[SE]->rect = rectSE;
            quad->noeuds[SE]->parent = quad;
            quad->noeuds[SE]->first = quad->first;
        }


        int nombre_noeuds = 0;
        //Compte dans combien de noeuds fils est contenu l'objet
        for (int i = 0; i < 4; i++)
        {
            if ( sfIntRect_Intersects(&quad->noeuds[i]->rect, &rect_obj, NULL) )
            {
                nombre_noeuds++;
            }
        }

        //Si il est contenu dans un seul noeud fils, on refait une passe à partir du noeud fils qui le contient
        if (nombre_noeuds == 1 && quad->rect.Right - quad->rect.Left > 100)
        {
            for (int i = 0; i < 4; i++)
                quadtree_Add(quad->noeuds[i], obj_, type);
        }
        else
        {
            //Cast
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
            else if (type == BULLET)
            {
                Bullet* obj = obj_;
                list_Add(quad->bullet, obj, type);
                obj->quad_node = quad;
            }
        }
    }

    sfMutex_Unlock(g_QuadTree_Access);
}

void quadtree_Print(QuadTree* quad)
{
    if (quad != NULL)
    {
        printf("%d %d %d %d\n", quad->rect.Left, quad->rect.Top, quad->rect.Right, quad->rect.Bottom);
        for (int i = 0; i < 4; i++)
            quadtree_Print(quad->noeuds[i]);
    }
}

//Fonction qui permet de dessiner le quad_tree
void quadtree_Draw(sfRenderWindow* Game, QuadTree* quad)
{
    //Simple parcours de l'arbre recursif qui créé un rectangle determiné par le rect contenu dans un noeud
    if (quad != NULL)
    {
        sfShape* test = NULL;
        if (quad->player != NULL && quad->player->taille > 0)
        {
            test = sfShape_CreateRectangle(quad->rect.Left+1, quad->rect.Top+1, quad->rect.Right-1, quad->rect.Bottom-1, sfColor_FromRGBA(255,255,255,0), 1, sfRed);
        }
        else
        {
            test = sfShape_CreateRectangle(quad->rect.Left + 1, quad->rect.Top + 1, quad->rect.Right - 1, quad->rect.Bottom - 1, sfColor_FromRGBA(255, 255, 255, 0), 1, sfWhite);
        }
        sfRenderWindow_DrawShape(Game, test);
        sfShape_Destroy(test);
        for (int i = 0; i < 4; i++)
            quadtree_Draw(Game, quad->noeuds[i]);
    }
}

//Genere un quad_tree à partir d'une Map
void quadtree_Generate(QuadTree* quad, Map* map)
{

    for (unsigned int i = 0; i < map->nb_objects; i++)
    {
        quadtree_Add(quad, map->objects_list[i], OBJECT);
    }

    for (unsigned int i = 0; i < map->nb_players; i++)
    {
        quadtree_Add(quad, map->players_list[i], PLAYER);
    }

    for (Bullet* ptr = BulletList_GetHead(map->bullets); ptr != NULL; ptr = bullet_GetNext(ptr))
    {
        quadtree_Add(quad, ptr, BULLET);
    }
}

//Supprime un element du quad_tree
void quadtree_Delete_Elt(void* obj_, int type)
{
    sfMutex_Lock(g_QuadTree_Access);

    if (type == OBJECT)
    {
        Object* obj = obj_;
        list_Delete(obj->quad_node->object, obj->list_node);
        quadtree_Delete_Node(obj->quad_node);
    }
    else if (type == PLAYER)
    {
        Player* obj = obj_;
        list_Delete(obj->quad_node->player, obj->list_node);
        quadtree_Delete_Node(obj->quad_node);
    }
    else if (type == BULLET)
    {
        Bullet* obj = obj_;
        list_Delete(obj->quad_node->bullet, obj->list_node);
        quadtree_Delete_Node(obj->quad_node);
    }

    sfMutex_Unlock(g_QuadTree_Access);
}

//Fonction qui vérifie si un noeud et ces fils sont vides
void quadtree_Check_Node(QuadTree* quad, bool* check)
{

    //Si on a deja trouvé un noeud non vide on s'arrete
    if (*check == 0)
        return;

    //Si les list de bullet/player/object ne sont pas NULL (noeuds pas/mal initialisé)
    if (quad != NULL && quad->bullet != NULL && quad->player != NULL && quad->object != NULL)
    {
        //On regarde si les noeuds ne contiennent rien
        if (quad->object->taille == 0 && quad->player->taille == 0 && quad->bullet->taille == 0)
        {
            //Si oui, on check les noeuds fils
            for (int i = 0; i < 4; i++)
                quadtree_Check_Node(quad->noeuds[i], check);
        }
        else
        {
            //Sinon on s'arrete
            *check = 0;
        }
    }
}

//Fonction qui check un noeud et le supprime s'il ne contient rien
void quadtree_Delete_Node(QuadTree* quad)
{
    bool test = true;

    // Vérifie que le noeud et ses fils sont vides
    quadtree_Check_Node(quad, &test);
    if (test)
    {
        // Supprime les noeuds fils
        for (int i = 0; i < 4; i++)
        {
            quadtree_Destroy(quad->noeuds[i]);
            quad->noeuds[i] = NULL;
        }

        // Supprime le noeud
        quadtree_Delete_Node(quad->parent);
    }
}

//Met à jour un objet dans un quad_tree
void quadtree_Update(void* obj_, int type)
{
    // Cast de obj_
    sfIntRect rect_obj;
    QuadTree* quad = NULL;

    sfMutex_Lock(g_QuadTree_Access);

    if (type == OBJECT)
    {
        Object* obj = obj_;
        quad = obj->quad_node->first;
        rect_obj = sprite_GetRect(obj->sprite);
    }
    else if (type == PLAYER)
    {
        Player* obj = obj_;
        quad = obj->quad_node->first;
        rect_obj = sprite_GetRect(obj->sprite);
    }
    else
    {
        Bullet* obj = obj_;
        quad = obj->quad_node->first;
        rect_obj = sprite_GetRect(obj->draw_image);
    }


    /*if (!IntRect_Contains(&quad->rect, &rect_obj))
    {*/
    quadtree_Delete_Elt(obj_, type);
    quadtree_Add(quad, obj_, type);
    // }

    sfMutex_Unlock(g_QuadTree_Access);
}

//Fonction qui determine si un rect est contenu dans un autre
bool IntRect_Contains(sfIntRect* rect, sfIntRect* rect2)
{

    if (sfIntRect_Intersects(rect, rect2, NULL))
        if (rect->Top <= rect2->Top && rect->Bottom >= rect2->Bottom && rect->Left <= rect2->Left && rect->Right >= rect2->Right)
            return true;
        else
            return false;
    else
        return false;

}
