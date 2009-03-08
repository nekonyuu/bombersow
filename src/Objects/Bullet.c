#include <assert.h>
#include "Objects/Bullet.h"
#include "Objects/Weapon.h"

Bullet* bullet_Create(unsigned int type)
{
    Bullet* new_bullet = NULL;
    assert(new_bullet = (Bullet*) malloc(sizeof(Bullet)));

    new_bullet->bullet_type = type;
    new_bullet->trajectory = armory[type].bullet->trajectory;
    new_bullet->draw_image = armory[type].bullet->draw_image;

    return new_bullet;
}

void bullet_Destroy(Bullet* bullet2destroy)
{
    assert(bullet2destroy != NULL);
    bullet2destroy->draw_image = NULL;
    free(bullet2destroy);
}
