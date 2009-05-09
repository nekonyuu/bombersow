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

#include <SFML/Graphics.h>
#include "BaseSystem/Logging.h"
#include "Objects/GameObjects.h"

// Fonction qui gère les touches pour les mouvements/tirs des joueurs
// TODO : Bug détection inputs + events....
void control_PlayerControl(void* UserData)
{
    ControlData* ctrl_player = (ControlData*) UserData;
    sfInput* keys_input;
    sfEvent Event;
    sfClock* timer = sfClock_Create();
    float elapsed_time;

    do
    {
        elapsed_time = sfClock_GetTime(timer);
        sfClock_Reset(timer);

        keys_input = sfRenderWindow_GetInput(ctrl_player->App);

        if (sfInput_IsKeyDown(keys_input, sfKeyD))                   // Touche D appuyée
        {
            player_Displace(ctrl_player->player, RIGHT, elapsed_time, ctrl_player->config);
        }
        if (sfInput_IsKeyDown(keys_input, sfKeyQ))                   // Touche Q appuyée
        {
            player_Displace(ctrl_player->player, LEFT, elapsed_time, ctrl_player->config);
        }

        while (sfRenderWindow_GetEvent(ctrl_player->App, &Event))   // Tant qu'il se passe quelque chose sur les entrées
        {
            if (Event.Type == sfEvtKeyPressed)
            {
                // Saut
                if (Event.Key.Code == sfKeyZ)
                    player_Displace(ctrl_player->player, UP, 0, ctrl_player->config);

                // Descente rapide
                if (Event.Key.Code == sfKeyS)
                    player_Displace(ctrl_player->player, DOWN, 0, ctrl_player->config);

                // Menu
                if (Event.Key.Code == sfKeyEscape)
                {
                    // TODO : Boite de dialogue quitte y/n, ou bien sur le menu
                    ctrl_player->ingame = false;
                }
            }

            if(Event.Type == sfEvtClosed)
                ctrl_player->ingame = false;

            // Tir
            /*if (Event.Type == sfEvtMouseButtonPressed && Event.MouseButton.Button == sfButtonLeft)
                player_WeaponShoot(ctrl_player->map, ctrl_player->player);*/
        }
        //sfSleep(1.f/1000.f);
    }
    while (ctrl_player->ingame);                                    // Tant qu'on est dans une partie
}
