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
    _Bool ingame = true;

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

    int i = 0;
    for (i = 0; i < 25; i++)
    {
        Sprite* spr = sprite_Create(0, 0, image_animation, NULL);
        Player* plr = player_Create("HAHA", CROWBAR);
        plr->sprite = spr;
        player_SetPosition(plr, (i-i%10)*5, (i%10)*50);
        map_AddPlayer(map, plr);
    }

    for (i = 50; i < 76; i++)
    {
        Animation* ani = animation_Create(image_animation2, 0, 0, 30, 30, 4, 0, BOUCLE, 0.1f);
        Object* obj_temp = object_Create(0);
        object_LoadImg(obj_temp, NULL, ani);
        object_SetPosition(obj_temp, (i-i%10)*5, (i%10)*50);
        map_AddObject(map, obj_temp);
    }

    sfClock* clock = sfClock_Create();
    int fps = 0;

    // Vidage des Events qui trainent dans la sfRenderWindow
    while (sfRenderWindow_GetEvent(Game, &Event));

    do
    {
        if (sfClock_GetTime(clock) > 1)
        {
            printf("%d\n", fps);
            sfClock_Reset(clock);
            fps = 0;
        }
        fps++;

        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'�cran

        gravitysystem_WorldUpdate(map, config);
        map_Draw(Game, map);
        //quad_tree_Draw(Game, map->quad_tree);

        sfRenderWindow_Display(Game);                           // Mise � jour de la fen�tre

        control_PlayerControl(Game, map, map->players_list[0], config, &ingame);
    }
    while (ingame);

    map_Destroy(map);

    sfImage_Destroy(image_animation);

    return false;
}
