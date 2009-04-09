#include <stdio.h>
#include "BaseSystem/Logging.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "PhysicsEngine/CollisionSystem.h"
#include "Objects/GameObjects.h"

Collision* collision_Create(){

    Collision* collision = NULL;
    assert(collision = (Collision*)malloc(sizeof(Collision)));

    collision->type = 0;
    collision->object = NULL;
    collision->player = NULL;
    collision->bullet = NULL;

    return collision;
}

void collision_Destroy(Collision* collision){

    free_secure(collision);
    collision = NULL;

}

Collision* collision_Detection_Object(void* obj_, int type){

    Quad_tree* node = NULL;
    sfIntRect rect_obj;

    if(type == OBJECT){
        Object* obj = obj_;
        rect_obj = sprite_GetRect(obj->sprite);
        node = obj->quad_node;
    }else if(type == PLAYER){
        Player* obj = obj_;
        rect_obj = sprite_GetRect(obj->sprite);
        node = obj->quad_node;
    }else{
        Bullet* obj = obj_;
        rect_obj = sprite_GetRect(obj->draw_image);
        node = obj->quad_node;
    }


    List_element* list_temp = node->object->first;

    Object* object_temp = NULL;
    Player* player_temp = NULL;
    Bullet* bullet_temp = NULL;

    Collision* collision = collision_Create();

    while(node != NULL){

        list_temp = (node->object != NULL) ? node->object->first : NULL;
        while(list_temp != NULL && list_temp->elt != NULL){
            object_temp = list_temp->elt;
            sfIntRect rect_obj2 = sprite_GetRect(object_temp->sprite);
            if( object_temp != obj_ && sfIntRect_Intersects(&rect_obj, &rect_obj2, NULL) ){
                collision->type = OBJECT;
                collision->object = object_temp;
                return collision;
            }

            list_temp = list_temp->next;
        }


        list_temp = (node->bullet != NULL) ? node->bullet->first : NULL;
        while(list_temp != NULL && list_temp->elt != NULL){
            bullet_temp = list_temp->elt;
            sfIntRect rect_obj2 = sprite_GetRect(bullet_temp->draw_image);
            if( sfIntRect_Intersects(&rect_obj, &rect_obj2, NULL) ){
                collision->type = BULLET;
                collision->bullet = bullet_temp;
                return collision;
            }

            list_temp = list_temp->next;
        }

        list_temp = (node->player != NULL) ? node->player->first : NULL;
        while(list_temp != NULL && list_temp->elt != NULL){
            player_temp = list_temp->elt;
            sfIntRect rect_obj2 = sprite_GetRect(player_temp->sprite);
            if( sfIntRect_Intersects(&rect_obj, &rect_obj2, NULL) ){
                collision->type = PLAYER;
                collision->player = player_temp;
                return collision;
            }

            list_temp = list_temp->next;
        }
        node = node->parent;
    }

    collision_Destroy(collision);
    return NULL;

}

