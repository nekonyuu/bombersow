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

// Fonction qui g�re les touches pour les mouvements/tirs des joueurs
inline void control_PlayerControl(sfRenderWindow* App, Map* map, Player* player, Config* config, bool* ingame)
{
    sfInput* keys_input;
    sfEvent Event;
    float elapsed_time;

    elapsed_time = sfRenderWindow_GetFrameTime(App);

    keys_input = sfRenderWindow_GetInput(App);

    if (sfInput_IsKeyDown(keys_input, sfKeyD))                   // Touche D appuy�e
    {
        player_Displace(player, RIGHT, elapsed_time, config);
    }
    if (sfInput_IsKeyDown(keys_input, sfKeyQ))                   // Touche Q appuy�e
    {
        player_Displace(player, LEFT, elapsed_time, config);
    }

    while (sfRenderWindow_GetEvent(App, &Event))   // Tant qu'il se passe quelque chose sur les entr�es
    {
        if (Event.Type == sfEvtKeyPressed)
        {
            // Saut
            if (Event.Key.Code == sfKeyZ)
                player_Displace(player, UP, 0, config);

            /* Y A PAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAASSS
            // Descente rapide
            if (Event.Key.Code == sfKeyS)
                player_Displace(player, DOWN, 0, config);
            */

            // Menu
            if (Event.Key.Code == sfKeyEscape)
            {
                // TODO : Boite de dialogue quitte y/n, ou bien sur le menu
                *ingame = false;
            }

            if (Event.Key.Code == sfKeyF10)
            {
                sfImage* screen = sfRenderWindow_Capture(App);
                sfImage_SaveToFile(screen, "capture.bmp");
                sfImage_Destroy(screen);
            }

            if (Event.Key.Code == sfKeyNum1)
                player_SwitchWeapon(player, CROWBAR);

            if (Event.Key.Code == sfKeyNum2)
                player_SwitchWeapon(player, SHOTGUN);

            if (Event.Key.Code == sfKeyNum3)
                player_SwitchWeapon(player, ROCKET_LAUNCHER);
            if (Event.Key.Code == sfKeyNum4)
                player_SwitchWeapon(player, MACHINEGUN);
            if (Event.Key.Code == sfKeyNum5)
                player_SwitchWeapon(player, GRENADES);
            if (Event.Key.Code == sfKeyNum6)
                player_SwitchWeapon(player, SNIPER);
            if (Event.Key.Code == sfKeyNum7)
                player_SwitchWeapon(player, LASERGUN);
        }

        if (Event.Type == sfEvtClosed)
            *ingame = false;

        // Tir
        if (Event.Type == sfEvtMouseButtonPressed && Event.MouseButton.Button == sfButtonLeft)
            player_WeaponShoot(map, player, sfInput_GetMouseX(keys_input), sfInput_GetMouseY(keys_input));
    }
}
