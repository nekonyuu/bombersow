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
#include "Game/GameScreens.h"
#include "GraphicEngine/Draw.h"
#include "Objects/Screen.h"
#include "Objects/GameObjects.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "PhysicsEngine/CollisionSystem.h"
#include "PhysicsEngine/GravitySystem.h"

_Bool display_Playing(sfRenderWindow* Game, Config* config)
{
    sfImage *image_animation = sfImage_CreateFromFile("base/images/animation2.png"); // Test
    sfImage *image_animation2 = sfImage_CreateFromFile("base/images/animation.png"); // Test

    sfEvent Event;

    Animation *animation = animation_Create(image_animation2, 0, 0, 30, 30, 4, 0, BOUCLE, 0.1f);
    Animation *animation2 = animation_Create(image_animation2, 0, 0, 30, 30, 4, 0, BOUCLE, 0.1f);

    Object* obj_temp = object_Create(0);
    object_LoadImg(obj_temp, NULL, animation);
    object_SetPosition(obj_temp, 800, 560);

    Object* obj_temp2 = object_Create(0);
    object_LoadImg(obj_temp2, NULL, animation2);
    object_SetPosition(obj_temp2, 800, 230);

    Map* map = map_Create(0, 100, config);
    map_AddObject(map, obj_temp);
    map_AddObject(map, obj_temp2);

    Player* player_tab[100];

    for (int i = 0; i < 100; i++)
    {
        Sprite* spr = sprite_Create(0, 0, image_animation, NULL);

        player_tab[i] = player_Create("HAHA", CROWBAR);
        player_tab[i]->sprite = spr;
        player_SetPosition(player_tab[i], (i-i%10)*5, (i%10)*50);
        map_AddPlayer(map, player_tab[i]);
    }

    sfClock* clock = sfClock_Create();
    int fps = 0;

    // Vidage des Events qui trainent dans la sfRenderWindow
    while (sfRenderWindow_GetEvent(Game, &Event));

    ControlData* player_data = (ControlData*) malloc(sizeof(ControlData));
    player_data->App = Game;
    player_data->map = map;
    player_data->player = player_tab[0];
    player_data->config = config;
    player_data->ingame = true;
    sfThread* player_control = sfThread_Create(&control_PlayerControl, player_data);
    Control_DrawMutex = sfMutex_Create();
    //sfThread_Launch(player_control);

    sfInput* keys_input;
    sfClock* timer = sfClock_Create();
    float elapsed_time;

    do
    {
        if (sfClock_GetTime(clock) > 1)
        {
            printf("%d\n", fps);
            sfClock_Reset(clock);
            fps = 0;
        }
        fps++;

        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran

        gravitysystem_WorldUpdate(map, config);
        map_Draw(Game, map);
        //quad_tree_Draw(Game, map->quad_tree);

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre

        // TODO : Temporaire, Bug anormal dans l'accès par le thread a la fenêtre
        elapsed_time = sfClock_GetTime(timer);
        sfClock_Reset(timer);

        keys_input = sfRenderWindow_GetInput(player_data->App);

        if (sfInput_IsKeyDown(keys_input, sfKeyD))                   // Touche D appuyée
        {
            player_Displace(player_data->player, RIGHT, elapsed_time, player_data->config);
        }
        if (sfInput_IsKeyDown(keys_input, sfKeyQ))                   // Touche Q appuyée
        {
            player_Displace(player_data->player, LEFT, elapsed_time, player_data->config);
        }

        while (sfRenderWindow_GetEvent(player_data->App, &Event))   // Tant qu'il se passe quelque chose sur les entrées
        {
            if (Event.Type == sfEvtKeyPressed)
            {
                // Saut
                if (Event.Key.Code == sfKeyZ)
                    player_Displace(player_data->player, UP, 0, player_data->config);

                // Descente rapide
                if (Event.Key.Code == sfKeyS)
                    player_Displace(player_data->player, DOWN, 0, player_data->config);

                // Menu
                if (Event.Key.Code == sfKeyEscape)
                {
                    // TODO : Boite de dialogue quitte y/n, ou bien sur le menu
                    player_data->ingame = false;
                }
            }

            if(Event.Type == sfEvtClosed)
                player_data->ingame = false;

            // Tir
            /*if (Event.Type == sfEvtMouseButtonPressed && Event.MouseButton.Button == sfButtonLeft)
                player_WeaponShoot(player_data->map, player_data->player);*/
        }

    }
    while (player_data->ingame);

    //sfThread_Wait(player_control);
    //sfThread_Destroy(player_control);

    free_secure(player_data);

    map_Destroy(map);

    sfImage_Destroy(image_animation);

    return false;
}
