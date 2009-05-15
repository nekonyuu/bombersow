/*
    GPL v3 Licence :
    Bombersow is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bombersow is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bombersow.  If not, see <http://www.gnu.org/licenses/>.


    Creative Commons BY-NC-SA :
    This work is licensed under the Creative Commons Attribution-Noncommercial-Share Alike 3.0 Unported License.
    To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter
    to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.

*/

#include "BaseSystem/Logging.h"
#include "Objects/GameObjects.h"

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
    new_weapon->respawn_time = armory[type].respawn_time;

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
        logging_Warning("weapon_Destroy", "Weapon object sent NULL");
    else
    {
        weapon2destroy->name = NULL;
        weapon2destroy->weapon_img = NULL;
        weapon2destroy->bullet_img = NULL;
        free_secure(weapon2destroy);
    }
}

// Création de l'armurerie & WeaponDefs, TODO : Chargement des sprites des armes
void armory_Create(Weapon* armory)
{
    // Pied de biche
    logging_Info("armory_Create", "Load Crowbar");
    armory[CROWBAR].name = sfString_Create();
    sfString_SetText(armory[CROWBAR].name, "Crowbar");
    armory[CROWBAR].type = CROWBAR;

    armory[CROWBAR].weapon_img = NULL;
    armory[CROWBAR].bullet_img = NULL;

    armory[CROWBAR].nb_max_bullets = -1;
    armory[CROWBAR].nb_curr_bullets = -1;
    armory[CROWBAR].damage = 20;

    armory[CROWBAR].reload_latency = 1000;
    armory[CROWBAR].switch_latency = 800;
    //armory[CROWBAR].respawn_time = ;

    armory[CROWBAR].range = 20;
    armory[CROWBAR].splash_radius = 0;
    armory[CROWBAR].splash_coef = 0;
    armory[CROWBAR].selfdamage_coef = 0;

    armory[CROWBAR].proj_speed = -1;
    armory[CROWBAR].trajectory = 0;

    armory[CROWBAR].collected = false;


    // Lance Roquettes
    logging_Info("armory_Create", "Load Rocket-Launcher");
    armory[ROCKET_LAUNCHER].name = sfString_Create();
    sfString_SetText(armory[ROCKET_LAUNCHER].name, "Rocket-Launcher");
    armory[ROCKET_LAUNCHER].type = ROCKET_LAUNCHER;

    armory[ROCKET_LAUNCHER].weapon_img = NULL;
    armory[ROCKET_LAUNCHER].bullet_img = NULL;

    armory[ROCKET_LAUNCHER].nb_max_bullets = 40;
    armory[ROCKET_LAUNCHER].nb_curr_bullets = 0;
    armory[ROCKET_LAUNCHER].damage = 65;

    armory[ROCKET_LAUNCHER].reload_latency = 800;
    armory[ROCKET_LAUNCHER].switch_latency = 800;
    //armory[ROCKET_LAUNCHER].respawn_time = ;

    armory[ROCKET_LAUNCHER].range = 2000;
    armory[ROCKET_LAUNCHER].splash_radius = 26;
    armory[ROCKET_LAUNCHER].splash_coef = 0.95;
    armory[ROCKET_LAUNCHER].selfdamage_coef = 0.6;

    armory[ROCKET_LAUNCHER].proj_speed = 350;
    armory[ROCKET_LAUNCHER].trajectory = 0;

    armory[ROCKET_LAUNCHER].collected = false;


    // Grenades (explosion 3500ms)
    logging_Info("armory_Create", "Load Grenade-Launcher");
    armory[GRENADES].name = sfString_Create();
    sfString_SetText(armory[GRENADES].name, "Grenades");
    armory[GRENADES].type = GRENADES;

    armory[GRENADES].weapon_img = NULL;
    armory[GRENADES].bullet_img = NULL;

    armory[GRENADES].nb_max_bullets = 20;
    armory[GRENADES].nb_curr_bullets = 0;
    armory[GRENADES].damage = 80;

    armory[GRENADES].reload_latency = 1500;
    armory[GRENADES].switch_latency = 500;
    //armory[GRENADES].respawn_time = ;

    armory[GRENADES].range = 2000;
    armory[GRENADES].splash_radius = 50;
    armory[GRENADES].splash_coef = 0.97;
    armory[GRENADES].selfdamage_coef = 0.9;

    armory[GRENADES].proj_speed = 200;
    armory[GRENADES].trajectory = 1;

    armory[GRENADES].collected = false;


    // Fusil à pompe
    logging_Info("armory_Create", "Load Shotgun");
    armory[SHOTGUN].name = sfString_Create();
    sfString_SetText(armory[SHOTGUN].name, "Shotgun");
    armory[SHOTGUN].type = SHOTGUN;

    armory[SHOTGUN].weapon_img = NULL;
    armory[SHOTGUN].bullet_img = NULL;

    armory[SHOTGUN].nb_max_bullets = 20;					// Nombre de cartouches, soit 140 projectiles
    armory[SHOTGUN].nb_curr_bullets = 0;
    armory[SHOTGUN].damage = 13;							// Dommage pour une cartouche

    armory[SHOTGUN].reload_latency = 1000;
    armory[SHOTGUN].switch_latency = 1000;
    //armory[SHOTGUN].respawn_time = ;

    armory[SHOTGUN].range = 150;
    armory[SHOTGUN].splash_radius = 0;
    armory[SHOTGUN].splash_coef = 1;
    armory[SHOTGUN].selfdamage_coef = 0;

    armory[SHOTGUN].proj_speed = -1;
    armory[SHOTGUN].trajectory = 2;

    armory[SHOTGUN].collected = false;


    // Mitrailleuse
    logging_Info("armory_Create", "Load MachineGun");
    armory[MACHINEGUN].name = sfString_Create();
    sfString_SetText(armory[MACHINEGUN].name, "MachineGun");
    armory[MACHINEGUN].type = MACHINEGUN;

    armory[MACHINEGUN].weapon_img = NULL;
    armory[MACHINEGUN].bullet_img = NULL;

    armory[MACHINEGUN].nb_max_bullets = 200;
    armory[MACHINEGUN].nb_curr_bullets = 0;
    armory[MACHINEGUN].damage = 10;

    armory[MACHINEGUN].reload_latency = 100;
    armory[MACHINEGUN].switch_latency = 0;
    //armory[MACHINEGUN].respawn_time = ;

    armory[MACHINEGUN].range = 2000;
    armory[MACHINEGUN].splash_radius = 5;
    armory[MACHINEGUN].splash_coef = 0.85;
    armory[MACHINEGUN].selfdamage_coef = 0;

    armory[MACHINEGUN].proj_speed = 600;
    armory[MACHINEGUN].trajectory = 0;

    armory[MACHINEGUN].collected = false;


    // Sniper
    logging_Info("armory_Create", "Load Sniper");
    armory[SNIPER].name = sfString_Create();
    sfString_SetText(armory[SNIPER].name, "Sniper");
    armory[SNIPER].type = SNIPER;

    armory[SNIPER].weapon_img = NULL;
    armory[SNIPER].bullet_img = NULL;

    armory[SNIPER].nb_max_bullets = 5;
    armory[SNIPER].nb_curr_bullets = 0;
    armory[SNIPER].damage = 100;

    armory[SNIPER].reload_latency = 1500;
    armory[SNIPER].switch_latency = -1;
    //armory[SNIPER].respawn_time = ;

    armory[SNIPER].range = 2000;
    armory[SNIPER].splash_radius = 0;
    armory[SNIPER].splash_coef = 1;
    armory[SNIPER].selfdamage_coef = 0;

    armory[SNIPER].proj_speed = 1200;
    armory[SNIPER].trajectory = 0;

    armory[SNIPER].collected = false;


    //Lasergun
    logging_Info("armory_Create", "Load Lasergun");
    armory[LASERGUN].name = sfString_Create();
    sfString_SetText(armory[LASERGUN].name, "Lasergun");
    armory[LASERGUN].type = LASERGUN;

    armory[LASERGUN].weapon_img = NULL;
    armory[LASERGUN].bullet_img = NULL;

    armory[LASERGUN].nb_max_bullets = 500;
    armory[LASERGUN].nb_curr_bullets = 0;
    armory[LASERGUN].damage = 10;

    armory[LASERGUN].reload_latency = 50;
    armory[LASERGUN].switch_latency = -1;
    //armory[LASERGUN].respawn_time = ;

    armory[LASERGUN].range = 200;
    armory[LASERGUN].splash_radius = 0;
    armory[LASERGUN].splash_coef = 1;
    armory[LASERGUN].selfdamage_coef = 0;

    armory[LASERGUN].proj_speed = -1;
    armory[LASERGUN].trajectory = 0;

    armory[LASERGUN].collected = false;

    logging_Info("armory_Create", "Armory loaded");
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
            sprite_Destroy(armory2destroy[i].bullet_img);
        }
    }
}
