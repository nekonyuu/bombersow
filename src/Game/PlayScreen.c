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
#include "Objects/Screen.h"
#include "GraphicEngine/Draw.h"
#include "GraphicEngine/Image.h"
#include "Objects/GameObjects.h"

_Bool display_PlayMenu(sfRenderWindow* Game, sfImage* BG_image, sfFont* playFont, Config* config)
{
    Screen* playMenu = screen_Create();
    sfEvent Event;
    _Bool launched = true, close = false;
    int menu_select = 1;

    screen_LoadImage(playMenu, BG_image);                           // Chargement de l'arrière-plan
    screen_LoadFont(playMenu, playFont);                            // Chargement de la police d'écriture
    // Préparation des textes
    screen_LoadText(playMenu, GAME_NAME, sfRed, 60, sfStringRegular, 450.0f, 40.0f);
    screen_LoadText(playMenu, "Rejoindre une partie", sfRed, 35, sfStringItalic, 450.0f, 140.0f);
    screen_LoadText(playMenu, "Créer un serveur", sfWhite, 35, sfStringItalic, 450.0f, 190.0f);
    screen_LoadText(playMenu, "Retour", sfWhite, 35, sfStringItalic, 450.0f, 240.0f);

    logging_Info("display_PlayMenu", "Started without error");

    do
    {
        sfRenderWindow_Clear(Game, sfBlack);                        // Vidage de l'écran

        sfRenderWindow_DrawSprite(Game, playMenu->images[0]);       // Dessin du BG

        for (int i = 0; i < playMenu->nb_text; i++)
            screen_DrawText(Game, playMenu, i);                     // Dessin des textes

        sfRenderWindow_Display(Game);                               // Mise à jour de la fenêtre

        while (sfRenderWindow_GetEvent(Game, &Event))               // Surveillance des évènements
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed)
            {
                close = true;                                       // Fermeture du Menu
                launched = false;
            }

            if (Event.Type == sfEvtKeyPressed)
            {
                if (Event.Key.Code == sfKeyEscape)                  // Echap : Reviens en arrière
                {
                    launched = false;
                }
                else if (Event.Key.Code == sfKeyReturn)
                {
                    switch (menu_select)
                    {
                    case 1:                                         // Ecran de connexion à un serveur
                        if (display_ClientMenu(Game, BG_image, playFont, config))
                        {
                            launched = false;
                            close = true;
                        }
                        break;
                    case 2:
                        if (display_ServerMenu(Game, BG_image, playFont))
                        {
                            launched = false;
                            close = true;
                        }
                        break;
                    case 3:
                        launched = false;
                        break;
                    }
                }
                else if (Event.Key.Code == sfKeyUp && menu_select > 1)
                {
                    sfString_SetColor(playMenu->texts[menu_select], sfWhite);
                    sfString_SetColor(playMenu->texts[--menu_select], sfRed);
                }
                else if (Event.Key.Code == sfKeyDown && menu_select < 3)
                {
                    sfString_SetColor(playMenu->texts[menu_select], sfWhite);
                    sfString_SetColor(playMenu->texts[++menu_select], sfRed);
                }
            }
        }
    }
    while (launched);

    screen_Destroy(playMenu);

    return close;
}
