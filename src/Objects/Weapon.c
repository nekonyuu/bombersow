#include <assert.h>

#include "Weapon.h"

// Contructeur
Weapon* weapon_Create(int type)
{
    Weapon* new_weapon = NULL;
    assert(new_weapon = (Weapon*) malloc(sizeof(Weapon)));

    // TODO

    return new_weapon;
}

// Destructeur
void weapon_Destroy(Weapon* weapon2destroy)
{
    object_Destroy(weapon2destroy->weapon_img);
    object_Destroy(weapon2destroy->bullet);
    sfString_Destroy(weapon2destroy->name);

    free(weapon2destroy);
}
