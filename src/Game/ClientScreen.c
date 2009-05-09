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
#include "Objects/Screen.h"
#include "GraphicEngine/Draw.h"
#include "GraphicEngine/Image.h"
#include "Gui/Gui.h"
#include "Networking/Networking.h"

_Bool display_ClientMenu(sfRenderWindow* Game, sfImage* BG_image, sfFont* playFont, Config* config)
{
    Screen* clientMenu = screen_Create();
    sfImage* textbox_bg = sfImage_CreateFromFile("base/images/gui/textbox_back.png");
    sfEvent Event;
    _Bool launched = true, close = false;
    int menu_select = 1, port = DEFAULT_PORT;
    char pseudo[20] = "Player", ip[20] = "127.0.0.1";

    screen_LoadImage(clientMenu, BG_image);                             // Chargement de l'arrière-plan
    screen_LoadFont(clientMenu, playFont);                              // Chargement de la police d'écriture
    // Préparation des textes
    screen_LoadText(clientMenu, GAME_NAME, sfRed, 60, sfStringRegular, 450.0f, 40.0f);
    screen_LoadText(clientMenu, "Pseudo", sfRed, 35, sfStringRegular, 450.0f, 140.0f);
    screen_LoadText(clientMenu, "IP", sfWhite, 35, sfStringRegular, 450.0f, 190.0f);
    screen_LoadText(clientMenu, "Port", sfWhite, 35, sfStringRegular, 450.0f, 240.0f);
    gui_Add_Textbox(clientMenu->gui, widget_textbox_Create(600, 150, 200, 25, 20, textbox_bg, sfBlack, CHAR, pseudo, "", sfBlack, playFont, 10));
    gui_Add_Textbox(clientMenu->gui, widget_textbox_Create(600, 200, 200, 25, 20, textbox_bg, sfBlack, CHAR, ip, "", sfBlack, playFont, 10));
    gui_Add_Textbox(clientMenu->gui, widget_textbox_Create(600, 250, 100, 25, 5, textbox_bg, sfBlack, INT, &port, "", sfBlack, playFont, 10));
    clientMenu->gui->widget_textbox[0]->active = true;

    logging_Info("display_ClientMenu", "Started without error");

    do
    {
        sfRenderWindow_Clear(Game, sfBlack);                            // Vidage de l'écran

        sfRenderWindow_DrawSprite(Game, clientMenu->images[0]);         // Dessin du BG

        for (int i = 0; i < clientMenu->nb_text; i++)
            screen_DrawText(Game, clientMenu, i);                       // Dessin des textes

        gui_Draw(Game, clientMenu->gui);                                // Dessin de la GUI
        sfRenderWindow_Display(Game);                                   // Mise à jour de la fenêtre

        while (sfRenderWindow_GetEvent(Game, &Event))                   // Surveillance des évènements
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed)
            {
                close = true;                                           // Fermeture du Menu
                launched = false;
            }

            if (Event.Type == sfEvtMouseButtonPressed)                  // Clic sur une textbox ?
            {
                gui_Click(clientMenu->gui, Event.MouseButton.X, Event.MouseButton.Y);
            }

            if (Event.Type == sfEvtTextEntered)                         // Entrée de texte
            {
                gui_TextEntered(clientMenu->gui, Event.Text.Unicode);
            }

            if (Event.Type == sfEvtKeyPressed)
            {
                switch (Event.Key.Code)
                {
                case sfKeyEscape:
                    launched = false;
                    break;

                case sfKeyReturn:
                    client_Main(pseudo, sfIPAddress_FromString(ip), port, config);
                    break;

                case sfKeyUp:
                    if (menu_select > 1)
                    {
                        sfString_SetColor(clientMenu->texts[menu_select], sfWhite);
                        clientMenu->gui->widget_textbox[menu_select - 1]->active = false;
                        sfString_SetColor(clientMenu->texts[--menu_select], sfRed);
                        clientMenu->gui->widget_textbox[menu_select - 1]->active = true;
                    }
                    break;

                case sfKeyDown:
                    if (menu_select < 3)
                    {
                        sfString_SetColor(clientMenu->texts[menu_select], sfWhite);
                        clientMenu->gui->widget_textbox[menu_select - 1]->active = false;
                        sfString_SetColor(clientMenu->texts[++menu_select], sfRed);
                        clientMenu->gui->widget_textbox[menu_select - 1]->active = true;
                    }
                    break;

                default:
                    break;
                }
            }
        }
    }
    while (launched);

    screen_Destroy(clientMenu);
    sfImage_Destroy(textbox_bg);

    return close;
}
