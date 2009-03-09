#include <assert.h>
#include <stdio.h>
#include "Objects/Weapon.h"

// Constructeur
Weapon* weapon_Create(int type)
{
    Weapon* new_weapon = NULL;
    assert(new_weapon = (Weapon*) malloc(sizeof(Weapon)));

    new_weapon->name = armory[type].name;

    new_weapon->weapon_img = armory[type].weapon_img;
    new_weapon->bullet_img = armory[type].bullet_img;

    new_weapon->nb_curr_bullets = armory[type].nb_curr_bullets;
    new_weapon->nb_max_bullets = armory[type].nb_max_bullets;
    new_weapon->damage = armory[type].damage;
    new_weapon->reload_latency = armory[type].reload_latency;
    new_weapon->switch_latency = armory[type].switch_latency;
    new_weapon->range = armory[type].range;
    new_weapon->splash_radius = armory[type].splash_radius;
    new_weapon->splash_coef = armory[type].splash_coef;
    new_weapon->selfdamage_coef = armory[type].selfdamage_coef;
    new_weapon->proj_speed = armory[type].proj_speed;
    new_weapon->trajectory = armory[type].trajectory;

    new_weapon->collected = false;

    return new_weapon;
}

// Destructeur
void weapon_Destroy(Weapon* weapon2destroy)
{
    if (!weapon2destroy)
    {
        printf("Warning - weapon_Destroy : Weapon object sent NULL");
        return;
    }
    weapon2destroy->name = NULL;
    weapon2destroy->weapon_img = NULL;
    weapon2destroy->bullet_img = NULL;

    free(weapon2destroy);
}

// Création de l'armurerie & WeaponDefs, TODO : Chargement des sprites des armes
void armory_Create(Weapon* armory)
{
    // Pied de biche
    armory[0].name = sfString_Create();
    sfString_SetText(armory[0].name, "Crowbar");

    armory[0].weapon_img = NULL;
    armory[0].bullet_img = NULL;

    armory[0].nb_max_bullets = -1;
    armory[0].nb_curr_bullets = -1;
    armory[0].damage = 20;
    armory[0].reload_latency = 1000;
    armory[0].switch_latency = 800;
    armory[0].range = 10;
    armory[0].splash_radius = 0;
    armory[0].splash_coef = 0;
    armory[0].selfdamage_coef = 0;
    armory[0].proj_speed = -1;
    armory[0].trajectory = 0;

    armory[0].collected = false;


    // Lance Roquettes
    armory[1].name = sfString_Create();
    sfString_SetText(armory[1].name, "Rocket-Launcher");

    armory[1].weapon_img = NULL;
    armory[1].bullet_img = NULL;

    armory[1].nb_max_bullets = 40;
    armory[1].nb_curr_bullets = 0;
    armory[1].damage = 65;
    armory[1].reload_latency = 800;
    armory[1].switch_latency = 800;
    armory[1].range = 2000;
    armory[1].splash_radius = 26;
    armory[1].splash_coef = 0.95;
    armory[1].selfdamage_coef = 0.6;
    armory[1].proj_speed = 350;
    armory[1].trajectory = 0;

    armory[1].collected = false;


    // Lance Grenades (explosion 3500ms)
    armory[2].name = sfString_Create();
    sfString_SetText(armory[2].name, "Grenade-Launcher");

    armory[2].weapon_img = NULL;
    armory[2].bullet_img = NULL;

    armory[2].nb_max_bullets = 20;
    armory[2].nb_curr_bullets = 0;
    armory[2].damage = 80;
    armory[2].reload_latency = 1500;
    armory[2].switch_latency = 500;
    armory[2].range = 2000;
    armory[2].splash_radius = 50;
    armory[2].splash_coef = 0.97;
    armory[2].selfdamage_coef = 0.9;
    armory[2].proj_speed = 200;
    armory[2].trajectory = 1;

    armory[2].collected = false;


    // Fusil à pompe
    armory[3].name = sfString_Create();
    sfString_SetText(armory[3].name, "Shotgun");

    armory[3].weapon_img = NULL;
    armory[3].bullet_img = NULL;

    armory[3].nb_max_bullets = 20;					// Nombre de cartouches, soit 140 projectiles
    armory[3].nb_curr_bullets = 0;
    armory[3].damage = 13;							// Dommage pour une cartouche
    armory[3].reload_latency = 1000;
    armory[3].switch_latency = 1000;
    armory[3].range = 150;
    armory[3].splash_radius = 0;
    armory[3].splash_coef = 1;
    armory[3].selfdamage_coef = 0;
    armory[3].proj_speed = -1;
    armory[3].trajectory = 2;

    armory[3].collected = false;


    // Mitrailleuse
    armory[4].name = sfString_Create();
    sfString_SetText(armory[4].name, "Minigun");

    armory[4].weapon_img = NULL;
    armory[4].bullet_img = NULL;

    armory[4].nb_max_bullets = 200;
    armory[4].nb_curr_bullets = 0;
    armory[4].damage = 10;
    armory[4].reload_latency = 100;
    armory[4].switch_latency = 0;
    armory[4].range = 2000;
    armory[4].splash_radius = 5;
    armory[4].splash_coef = 0.85;
    armory[4].selfdamage_coef = 0;
    armory[4].proj_speed = 600;
    armory[4].trajectory = 0;

    armory[4].collected = false;


    // Sniper
    armory[5].name = sfString_Create();
    sfString_SetText(armory[5].name, "Sniper");

    armory[5].weapon_img = NULL;
    armory[5].bullet_img = NULL;

    armory[5].nb_max_bullets = 5;
    armory[5].nb_curr_bullets = 0;
    armory[5].damage = 100;
    armory[5].reload_latency = 1500;
    armory[5].switch_latency = -1;
    armory[5].range = 2000;
    armory[5].splash_radius = 0;
    armory[5].splash_coef = 1;
    armory[5].selfdamage_coef = 0;
    armory[5].proj_speed = 1200;
    armory[5].trajectory = 0;

    armory[5].collected = false;


    //Lasergun
    armory[6].name = sfString_Create();
    sfString_SetText(armory[6].name, "Lasergun");

    armory[6].weapon_img = NULL;
    armory[6].bullet_img = NULL;

    armory[6].nb_max_bullets = 500;
    armory[6].nb_curr_bullets = 0;
    armory[6].damage = 10;
    armory[6].reload_latency = 50;
    armory[6].switch_latency = -1;
    armory[6].range = 200;
    armory[6].splash_radius = 0;
    armory[6].splash_coef = 1;
    armory[6].selfdamage_coef = 0;
    armory[6].proj_speed = -1;
    armory[6].trajectory = 0;

    armory[6].collected = false;
}

// Destructeur de l'armurerie
void armory_Destroy(Weapon* armory2destroy)
{
    for (int i = 0; i < NB_MAX_WEAPONS; i++)
    {
        if (armory2destroy[i].name != NULL && armory2destroy[i].weapon_img != NULL
                && armory2destroy[i].bullet_img != NULL)
        {
            sfString_Destroy(armory2destroy[i].name);
            sfSprite_Destroy(armory2destroy[i].weapon_img);
            sfSprite_Destroy(armory2destroy[i].bullet_img);
        }
    }
    free(armory2destroy);
}
