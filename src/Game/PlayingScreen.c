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

#include <string.h>
#include <SFML/Graphics.h>

#include "BaseSystem/Logging.h"
#include "Game/GameScreens.h"
#include "GraphicEngine/Draw.h"
#include "Objects/Screen.h"
#include "Objects/GameObjects.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "PhysicsEngine/CollisionSystem.h"
#include "PhysicsEngine/GravitySystem.h"
#include "PhysicsEngine/ParticleSystem.h"

bool display_Playing(sfRenderWindow* Game, Config* config)
{
    sfImage *image_animation = sfImage_CreateFromFile("base/images/animation2.png"); // Test
    sfImage *image_animation2 = sfImage_CreateFromFile("base/images/animation.png"); // Test
    bool ingame = true;

    Animation *animation = animation_Create(image_animation2, 0, 0, 30, 30, 4, 0, BOUCLE, 0.1f);
    Animation *animation2 = animation_Create(image_animation2, 0, 0, 30, 30, 4, 0, BOUCLE, 0.1f);

    logging_Info("display_Playing", "Create objects...");
    Object* obj_temp = object_Create(0);
    object_LoadImg(obj_temp, NULL, animation);
    object_SetPosition(obj_temp, 800, 560);

    Object* obj_temp2 = object_Create(0);
    object_LoadImg(obj_temp2, NULL, animation2);
    object_SetPosition(obj_temp2, 800, 230);

    logging_Info("display_Playing", "Create map...");
    Map* map = map_Create(0, 100, config);
    PhysicsEngine_Init();

    map_AddObject(map, obj_temp);
    map_AddObject(map, obj_temp2);

    int i = 0;
    logging_Info("display_Playing", "Create players...");
    for (i = 0; i < 35; i++)
    {
        char* name = malloc(5 * sizeof(char));
        strcpy(name, "HAHA");
        Player* plr = player_Create(name, CROWBAR);
        player_SetPosition(plr, (i - i % 3) * 15, (i % 10) * 50);
        map_AddPlayer(map, plr);
    }

    logging_Info("display_Playing", "Create pseudo-platform...");

    // Préparation des threads
    sfThread* phys_BloodUpdate = sfThread_Create(&gravitysystem_BloodUpdate, map);
    //sfThread* phys_PlayersUpdate = sfThread_Create(&gravitysystem_PlayerUpdate, map);
    //sfThread* phys_BulletsUpdate = sfThread_Create(&gravitysystem_BulletUpdate, map);

    map->game_started = true;

    // Démarrage
    sfThread_Launch(phys_BloodUpdate);
    //sfThread_Launch(phys_PlayersUpdate);
    //sfThread_Launch(phys_BulletsUpdate);

    do
    {
        GraphicEngine_ClearScreen(Game);

        gravitysystem_WorldUpdate(map);

        map_Draw(Game, map);                                    // Dessin de la map

        //quadtree_Draw(Game, map->quad_tree);

        if(config->show_fps)
            Screen_FPSShow(Game);

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre

        control_PlayerControl(Game, map, map->players_list[0], config, &ingame);
    }
    while (ingame);

    map->game_started = false;

    sfThread_Wait(phys_BloodUpdate);
    //sfThread_Wait(phys_BulletsUpdate);
    //sfThread_Wait(phys_PlayersUpdate);

    sfThread_Destroy(phys_BloodUpdate);
    //sfThread_Destroy(phys_BulletsUpdate);
    //sfThread_Destroy(phys_PlayersUpdate);

    map_Destroy(map);

    PhysicsEngine_Clean();

    sfImage_Destroy(image_animation);

    return false;
}
