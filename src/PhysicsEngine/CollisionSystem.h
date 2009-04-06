#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "Objects/GameObjects.h"
#include "PhysicsEngine/PhysicsEngine.h"

typedef struct COLLISION {

    Physics_type type;

    Object* object;
    Player* player;
    Bullet* bullet;

}Collision;

Collision* collision_Create();
void collision_Destroy(Collision*);
Collision* collision_Detection_Object(void*, int);

#endif
