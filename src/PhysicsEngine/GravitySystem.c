#include "PhysicsEngine/PhysicsEngine.h"
#include "PhysicsEngine/GravitySystem.h"
#include "Objects/GameObjects.h"
#include "Memleak/halloc.h"

void gravitysystem_WorldUpdate(Map* map_, float gravity)
{
    /*for (int i = 0; i < map_->nb_players; i++)
    {
        if(map_->players_list[i]->gravity)
        {

        }
    }*/

    for (int i = 0; i < map_->nb_objects; i++)
    {
        float y = gravity * sfClock_GetTime(map_->clock);
        if(map_->objects_list[i]->sprite->hauteur+map_->objects_list[i]->curr_coord_y+y < 480){
            object_SetPosition(map_->objects_list[i], map_->objects_list[i]->curr_coord_x, map_->objects_list[i]->curr_coord_y+y);
            quad_tree_Update(map_->objects_list[i], OBJECT);
        }
    }
    sfClock_Reset(map_->clock);
}
