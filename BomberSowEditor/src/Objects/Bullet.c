#include "BaseSystem/Logging.h"
#include "Objects/Bullet.h"
#include "Objects/Weapon.h"

Bullet* bullet_Create(unsigned int type)
{
    Bullet* new_bullet = NULL;
    assert(new_bullet = (Bullet*) malloc(sizeof(Bullet)));

    new_bullet->bullet_type = type;
    new_bullet->trajectory = armory[type].trajectory;
    new_bullet->draw_image = armory[type].bullet_img;

    return new_bullet;
}

void bullet_Destroy(Bullet* bullet2destroy)
{
    if(!bullet2destroy)
        logging_Error("bullet_Destroy", "Bullet object sent NULL");
    bullet2destroy->draw_image = NULL;
    free(bullet2destroy);
}
