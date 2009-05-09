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

void control_Playercontrols(sfRenderWindow* App, Map* map, Player* player_, Config* config)            // Fonction qui gère les touches pour les mouvements/tirs des joueurs
{
    _Bool ingame = true;
    sfEvent Event;
    sfInput* KeysInput;

    while (ingame)                                                                      // Tant qu'on est dans une partie
    {
        KeysInput = sfRenderWindow_GetInput(App);                                       // Récupération des entrées temps réel

        if (sfInput_IsKeyDown(KeysInput, sfKeyD))                                       // Touche D appuyée
            player_Displace(player_, RIGHT, sfRenderWindow_GetFrameTime(App), config);
        if (sfInput_IsKeyDown(KeysInput, sfKeyQ))                                       // Touche Q appuyée
            player_Displace(player_, LEFT, sfRenderWindow_GetFrameTime(App), config);

        while (sfRenderWindow_GetEvent(App, &Event))                                    // Tant qu'il se passe quelque chose sur les entrées
        {
            if (Event.Type == sfEvtKeyPressed)
            {
                switch (Event.Key.Code)
                {
                // Saut
                case sfKeyZ:
                    player_Displace(player_, UP, sfRenderWindow_GetFrameTime(App), config);
                    break;

                // Descente rapide
                case sfKeyS:
                    player_Displace(player_, DOWN, sfRenderWindow_GetFrameTime(App), config);
                    break;

                // Menu
                case sfKeyEscape:
                    // TODO : Boite de dialogue quitte y/n, ou bien sur le menu
                    break;

                // Autres appuis ignorés
                default:
                    break;
                }
            }
            // Tir
            if (Event.Type == sfEvtMouseButtonPressed && Event.MouseButton.Button == sfButtonLeft)
                player_WeaponShoot(map, player_);
        }
    }
}
