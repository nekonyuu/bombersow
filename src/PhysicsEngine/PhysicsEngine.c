#include "PhysicsEngine/PhysicsEngine.h"
#include "Objects/GameObjects.h"
#include <assert.h>
#include <stdio.h>

List* list_Create(){

    List* list = NULL;
    assert(list = (List*)malloc(sizeof(List)));

    list->previous = NULL;
    list->elt = NULL;
    list->next = NULL;

    return list;
}

void list_Add(List* list, void* elt){

    if(list->elt == NULL){
        list->elt = elt;
    }else{
        List* list2 = list_Create();
        list2->elt = elt;
        list2->next = list;
        list2->previous = NULL;
        list->previous = list2;
    }

}

void list_Delete(List* list){

    if(list->previous != NULL && list->next != NULL){
        list->next->previous = list->previous;
    }

    if(list->next != NULL && list->previous != NULL){
        list->previous->next = list->next;
    }

    free(list);
    list = NULL;

}

void list_Destroy(List* list){

    if(list != NULL){
        list_Destroy(list->next);
        free(list);
        list = NULL;
    }

}

Quad_tree* quad_tree_Create(){

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

    sfIntRect rect = {0,0,0,0};
    quad->rect = rect;

    return quad;

}
void quad_tree_Destroy(Quad_tree* quad){

    if(quad != NULL){
        for(int i = 0; i < 4; i++)
            quad_tree_Destroy(quad->noeuds[i]);

        list_Destroy(quad->object);
        list_Destroy(quad->bullet);
        list_Destroy(quad->player);
        free(quad);
        quad = NULL;
    }

}

void quad_tree_Add(Quad_tree* quad, void* obj_, int type){

    sfIntRect rect_obj;
    if(type == OBJECT){
        Object* obj = obj_;
        rect_obj = sprite_GetRect(obj->sprite);
    }else if(type == PLAYER){
        Player* obj = obj_;
        rect_obj = sprite_GetRect(obj->sprite);
    }else{
        Bullet* obj = obj_;
        rect_obj = sprite_GetRect(obj->draw_image);
    }

    if( sfIntRect_Intersects(&quad->rect, &rect_obj, NULL) ){

        if(quad->noeuds[NW] == NULL){
            sfIntRect rectNW = {quad->rect.Left, quad->rect.Top, (quad->rect.Right+quad->rect.Left)/2, (quad->rect.Bottom+quad->rect.Top)/2};
            quad->noeuds[NW] = quad_tree_Create();
            quad->noeuds[NW]->rect = rectNW;
            quad->noeuds[NW]->parent = quad;

            sfIntRect rectNE = {(quad->rect.Right+quad->rect.Left)/2, quad->rect.Top, quad->rect.Right, (quad->rect.Bottom+quad->rect.Top)/2};
            quad->noeuds[NE] = quad_tree_Create();
            quad->noeuds[NE]->rect = rectNE;
            quad->noeuds[NW]->parent = quad;

            sfIntRect rectSW = {quad->rect.Left, (quad->rect.Bottom+quad->rect.Top)/2, (quad->rect.Right+quad->rect.Left)/2, quad->rect.Bottom};
            quad->noeuds[SW] = quad_tree_Create();
            quad->noeuds[SW]->rect = rectSW;
            quad->noeuds[NW]->parent = quad;

            sfIntRect rectSE = {(quad->rect.Right+quad->rect.Left)/2, (quad->rect.Bottom+quad->rect.Top)/2, quad->rect.Right, quad->rect.Bottom};
            quad->noeuds[SE] = quad_tree_Create();
            quad->noeuds[SE]->rect = rectSE;
            quad->noeuds[NW]->parent = quad;
        }

        int nombre_noeuds = 0;
        for(int i = 0; i < 4; i++){
            if( sfIntRect_Intersects(&quad->noeuds[i]->rect, &rect_obj, NULL) ){
                nombre_noeuds++;
            }
        }

        if(nombre_noeuds == 1){
            for(int i = 0; i < 4; i++)
                quad_tree_Add(quad->noeuds[i], obj_, type);
        }else{

            if(type == OBJECT){
                Object* obj = obj_;

                if(quad->object == NULL)
                    quad->object = list_Create();

                list_Add(quad->object, obj);
                obj->quad_node = quad;
            }else if(type == PLAYER){
                Player* obj = obj_;

                if(quad->player == NULL)
                    quad->player = list_Create();

                list_Add(quad->player, obj);
                obj->quad_node = quad;

            }else{
                Bullet* obj = obj_;

                if(quad->bullet == NULL)
                    quad->bullet = list_Create();

                list_Add(quad->bullet, obj);
                obj->quad_node = quad;
            }

        }
    }

}

void quad_tree_Print(Quad_tree* quad){
    if(quad != NULL){
        printf("%d %d %d %d\n", quad->rect.Left, quad->rect.Top, quad->rect.Right, quad->rect.Bottom);
        for(int i = 0; i < 4; i++)
            quad_tree_Print(quad->noeuds[i]);
    }
}

void quad_tree_Draw(sfRenderWindow* Game, Quad_tree* quad){
    if(quad != NULL){
        sfShape* test = sfShape_CreateRectangle(quad->rect.Left+1, quad->rect.Top+1, quad->rect.Right-1, quad->rect.Bottom-1, sfColor_FromRGBA(255,255,255,0), 1, sfWhite);
        sfRenderWindow_DrawShape(Game, test);
        sfShape_Destroy(test);

        for(int i = 0; i < 4; i++)
            quad_tree_Draw(Game, quad->noeuds[i]);
    }
}

void quad_tree_Generate(Quad_tree* quad, Map* map){

    for(int i = 0; i < map->nb_objects; i++){
        quad_tree_Add(quad, map->objects_list[i], OBJECT);
    }

    for(int i = 0; i < map->nb_players; i++){
        quad_tree_Add(quad, map->players_list[i], PLAYER);
    }

    for(int i = 0; i < map->nb_bullets; i++){
        quad_tree_Add(quad, map->bullets_list[i], BULLET);
    }
}
