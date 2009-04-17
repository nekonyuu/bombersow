#include <SFML/Graphics.h>
#include "BaseSystem/Logging.h"
#include "Objects/GameObjects.h"

void control_Playercontrols(sfRenderWindow* App, Map* map, Player* player_ )            //Fonction qui gère les touches pour les mouvements/tirs des joueurs
{
    _Bool ingame = true;
    sfEvent Event;

    while (ingame)                                           //Tant qu'on est dans une partie
    {
        while (sfRenderWindow_GetEvent(App, &Event))                   //Tant qu'il se passe quelque chose
        {

            if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyZ) )
            {
                player_Jump(player_);

                if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyS) )   //Si le joueur appuie sur S alors qu'il appuie sur Z (quel connard) ça s'arrête
                    break;                                                             //Mais au final, ça continuera de sauter genre

                if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyD) )
                    player_Displace(player_, 0.16, 0.0);

                if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyQ) )
                    player_Displace(player_, -0.16, 0.0);
            }

            if (player_->jetpack_mode)                         //En mode jetpack
            {
                player_Displace(player_, 0, 0.33);

                if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyS) )    //CONNARD QUI APPUIE SUR DEUX TOUCHES
                {
                    break;
                }

                if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyQ) )    //Diagonale gauche
                {
                    player_Displace(player_, -0.33, 0.33);
                }

                if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyZ) )    //Diagonale droite
                {
                    player_Displace(player_, 0.33, 0.33);
                }
            }

            if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyS) && (player_->jetpack_mode) ) //commande uniquement disponible pour le jetpack
            {
                player_Displace(player_, 0.0, -0.33);

                if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyZ) )
                    break;

                if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyQ) )   //Pour le déplacement pendant qu'il tombe
                {
                    player_Displace(player_, -0.33, -0.33);                               //-X pour la gauche
                }

                if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyD) )
                {
                    player_Displace(player_, 0.33, -0.33);                                //+X pour la droite
                }
            }

            if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyQ) )       //Déplacement à gauche
            {
                player_Displace(player_, -0.16, 0.0);

                if ( (Event.Type == sfEvtKeyPressed) && ( (Event.Key.Code == sfKeyS) || (Event.Key.Code == sfKeyD) ) )
                    break;

                if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyZ) )    //Un saut avant
                {
                    player_Jump(player_);
                }

                if (player_->jetpack_mode)
                {
                    player_Displace(player_, -0.33, 0.0);

                    if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyD) )
                        break;

                    if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyS) )
                    {
                        player_Displace(player_, -0.33, -0.33);
                    }

                    if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyZ) )
                    {
                        player_Displace(player_, -0.33, 0.33);
                    }

                }
            }

            if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyD) )       //Déplacement à droite
            {
                player_Displace(player_, 0.16, 0.0);

                if ( (Event.Type == sfEvtKeyPressed) && ( (Event.Key.Code == sfKeyS) || (Event.Key.Code == sfKeyQ) ) )
                    break;

                if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyZ) )
                {
                    player_Jump(player_);
                }

                if (player_->jetpack_mode)
                {
                    player_Displace(player_, 0.33, 0.0);

                    if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyQ) )
                        break;

                    if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyS) )
                    {
                        player_Displace(player_, 0.33, -0.33);
                    }

                    if ( (Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyZ) )
                    {
                        player_Displace(player_, 0.33, 0.33);
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
