#include <assert.h>

#include "Objects/Weapon.h"

// Constructeur
Weapon* weapon_Create(int type)
{
    Weapon* new_weapon = NULL;
    assert(new_weapon = (Weapon*) malloc(sizeof(Weapon)));

    new_weapon->name = armory[type].name;

    new_weapon->weapon_img = armory[type].weapon_img;
    new_weapon->bullet = armory[type].bullet;

    new_weapon->nb_curr_bullets = armory[type].nb_curr_bullets;
    new_weapon->nb_max_bullets = armory[type].nb_max_bullets;
    new_weapon->damage = armory[type].damage;
    new_weapon->reload_latency = armory[type].reload_latency;
    new_weapon->range = armory[type].range;
    new_weapon->splash_radius = armory[type].splash_radius;
    new_weapon->splash_coef = armory[type].splash_coef;
    new_weapon->selfdamage_coef = armory[type].selfdamage_coef;
    new_weapon->proj_speed = armory[type].proj_speed;

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

// Création de l'armurerie & WeaponDefs
void armory_Create(Weapon* armory)
{
    // Pied de biche
    armory[0].name = sfString_Create();
    sfString_SetText(armory[0].name, "Crowbar");
    armory[0].weapon_img = NULL;
    armory[0].bullet = NULL;
    armory[0].nb_max_bullets = 0;
    armory[0].nb_curr_bullets = 0;
    armory[0].damage = 10;
    armory[0].reload_latency = 500;

    // Lance Roquettes
    armory[1].name = sfString_Create();
    sfString_SetText(armory[1].name, "Rocket-Launcher");

    // Lance Grenades
    armory[2].name = sfString_Create();
    sfString_SetText(armory[2].name, "Grenade-Launcher");

    // Fusil à pompe
    armory[3].name = sfString_Create();
    sfString_SetText(armory[3].name, "Shotgun");

    // Mitrailleuse
    armory[4].name = sfString_Create();
    sfString_SetText(armory[4].name, "Minigun");

    // Sniper
    armory[5].name = sfString_Create();
    sfString_SetText(armory[5].name, "Sniper");
}
