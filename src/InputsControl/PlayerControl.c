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

    while (ingame)                                                                      // Tant qu'on est dans une partie
    {
        while (sfRenderWindow_GetEvent(App, &Event))                                    // Tant qu'il se passe quelque chose
        {

            if (Event.Type == sfEvtKeyPressed && Event.Key.Code == sfKeyZ)
            {
                player_Jump(player_);
                if (player_->jetpack_mode)                                              // En mode jetpack
                {
                    player_Displace(player_, 0, 0.33, config);

                    if (Event.Type == sfEvtKeyPressed && Event.Key.Code == sfKeyS)      // CONNARD QUI APPUIE SUR DEUX TOUCHES
                        break;

                    if (Event.Type == sfEvtKeyPressed && Event.Key.Code == sfKeyQ)      // Diagonale gauche
                        player_Displace(player_, -0.33, 0.33, config);

                    if (Event.Type == sfEvtKeyPressed && Event.Key.Code == sfKeyZ)      // Diagonale droite
                        player_Displace(player_, 0.33, 0.33, config);
                }
                else
                {
                    if (Event.Type == sfEvtKeyPressed && Event.Key.Code == sfKeyS)      // Si le joueur appuie sur S alors qu'il appuie sur Z (quel connard) ça s'arrête
                        break;                                                          // Mais au final, ça continuera de sauter genre

                    if (Event.Type == sfEvtKeyPressed && Event.Key.Code == sfKeyD)
                        player_Displace(player_, 0.16, 0.0, config);

                    if (Event.Type == sfEvtKeyPressed && Event.Key.Code == sfKeyQ)
                        player_Displace(player_, -0.16, 0.0, config);
                }
            }

            if (Event.Type == sfEvtKeyPressed && player_->jetpack_mode) //commande uniquement disponible pour le jetpack
            {
                if (Event.Key.Code == sfKeyS)
                    player_Displace(player_, 0.0, -0.33, config);

                if (Event.Key.Code == sfKeyZ)
                    break;

                if (Event.Key.Code == sfKeyQ)                                           // Pour le déplacement pendant qu'il tombe
                    player_Displace(player_, -0.33, -0.33, config);                             // -X pour la gauche

                if (Event.Key.Code == sfKeyD)
                    player_Displace(player_, 0.33, -0.33, config);                              // +X pour la droite
            }

            if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyQ) )       //Déplacement à gauche
            {
                player_Displace(player_, -0.16, 0.0, config);

                if ( (Event.Type == sfEvtKeyPressed) && ( (Event.Key.Code == sfKeyS) || (Event.Key.Code == sfKeyD) ) )
                    break;

                if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyZ) )    //Un saut avant
                {
                    player_Jump(player_);
                }

                if (player_->jetpack_mode)
                {
                    player_Displace(player_, -0.33, 0.0, config);

                    if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyD) )
                        break;

                    if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyS) )
                    {
                        player_Displace(player_, -0.33, -0.33, config);
                    }

                    if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyZ) )
                    {
                        player_Displace(player_, -0.33, 0.33, config);
                    }

                }
            }

            if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyD) )       //Déplacement à droite
            {
                player_Displace(player_, 0.16, 0.0, config);

                if ( (Event.Type == sfEvtKeyPressed) && ( (Event.Key.Code == sfKeyS) || (Event.Key.Code == sfKeyQ) ) )
                    break;

                if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyZ) )
                {
                    player_Jump(player_);
                }

                if (player_->jetpack_mode)
                {
                    player_Displace(player_, 0.33, 0.0, config);

                    if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyQ) )
                        break;

                    if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyS) )
                    {
                        player_Displace(player_, 0.33, -0.33, config);
                    }

                    if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyZ) )
                    {
                        player_Displace(player_, 0.33, 0.33, config);
                    }

                }
            }

            if ( (Event.Type = sfEvtMouseButtonPressed) && (Event.MouseButton.Button == sfButtonLeft) )
            {
                player_WeaponShoot(map, player_);
            }

            if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyEscape) )
            {
                //TODO : Boite de dialogue quitte y/n, ou bien sur le menu
            }
        }
        sfSleep(0.025f);
    }
}

