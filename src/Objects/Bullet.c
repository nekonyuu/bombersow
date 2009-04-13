#include "BaseSystem/Logging.h"
#include "Objects/GameObjects.h"

Bullet* bullet_Create(unsigned int player_id, unsigned int type)
{
    Bullet* new_bullet = NULL;
    assert(new_bullet = (Bullet*) malloc(sizeof(Bullet)));

    new_bullet->owner = player_id;
    new_bullet->bullet_type = type;
    new_bullet->damage = armory[type].damage;
    new_bullet->trajectory = armory[type].trajectory;
    new_bullet->range = armory[type].range;
    new_bullet->draw_image = armory[type].bullet_img;

    new_bullet->quad_node = NULL;
    new_bullet->list_node = NULL;

    new_bullet->gravity = 0;
    new_bullet->acceleration = 0;

    return new_bullet;
}

void bullet_Destroy(Bullet* bullet2destroy)
{
    if (!bullet2destroy)
        logging_Error("bullet_Destroy", "Bullet object sent NULL");
    bullet2destroy->draw_image = NULL;
    free_secure(bullet2destroy);
}

void bullet_Draw(sfRenderWindow* Game, Bullet* bullet)
{
    sprite_Draw(Game, bullet->draw_image);
}
