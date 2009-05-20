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
#include "Gui/Gui.h"
#include "Networking/Networking.h"

bool display_ServerMenu(sfRenderWindow* Game, Config* config, sfImage* BG_image, sfFont* playFont)
{
    Screen* serverMenu = screen_Create();
    sfImage* textbox_bg = sfImage_CreateFromFile("base/images/gui/textbox_back.png");
    sfFont* textbox_font = sfFont_CreateFromFile("base/fonts/friday13v12.ttf", 50, NULL);
    sfEvent Event;
    bool launched = true, close = false;
    int menu_select = 1, port = DEFAULT_PORT;
    char pseudo[20] = "Player", servername[20] = "BomberSow Server";

    screen_LoadImage(serverMenu, BG_image);                             // Chargement de l'arrière-plan
    screen_LoadFont(serverMenu, playFont);                              // Chargement de la police d'écriture
    // Préparation des textes
    screen_LoadText(serverMenu, GAME_NAME, sfRed, 50, sfStringRegular, 450.0f, 40.0f);
    screen_LoadText(serverMenu, "Nom du serveur", sfRed, 20, sfStringRegular, 450.0f, 250.0f);
    screen_LoadText(serverMenu, "Pseudo", sfWhite, 20, sfStringRegular, 450.0f, 300.0f);
    screen_LoadText(serverMenu, "Port", sfWhite, 20, sfStringRegular, 450.0f, 350.0f);
    screen_LoadText(serverMenu, "Création du serveur", sfWhite, 35, sfStringRegular, 450.0f, 120.0f);
    screen_AddTextbox(serverMenu, 630, 250, 175, 20, 20, textbox_bg, sfBlack, CHAR, servername, sfBlack, "", sfBlack, textbox_font, 10);
    screen_AddTextbox(serverMenu, 630, 300, 175, 20, 20, textbox_bg, sfBlack, CHAR, pseudo, sfBlack, "", sfBlack, textbox_font, 10);
    screen_AddTextbox(serverMenu, 630, 350, 75, 20, 5, textbox_bg, sfBlack, INT, &port, sfBlack, "", sfBlack, playFont, 10);
    serverMenu->gui->widget_textbox[0]->active = true;

    logging_Info("display_ServerMenu", "Started without error");

    do
    {
        sfRenderWindow_Clear(Game, sfBlack);                            // Vidage de l'écran

        sfRenderWindow_DrawSprite(Game, serverMenu->images[0]);         // Dessin du BG

        for (int i = 0; i < serverMenu->nb_text; i++)
            screen_DrawText(Game, serverMenu, i);                       // Dessin des textes

        if(config->show_fps)
            logging_FPSShow(Game);

        screen_DrawGui(Game, serverMenu);                               // Dessin de la GUI
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
                gui_Click(serverMenu->gui, Event.MouseButton.X, Event.MouseButton.Y);
            }

            if (Event.Type == sfEvtTextEntered)                         // Entrée de texte
            {
                gui_TextEntered(serverMenu->gui, Event.Text.Unicode);
            }

            if (Event.Type == sfEvtKeyPressed)
            {
                if (Event.Key.Code == sfKeyEscape)                      // Echap : Reviens en arrière
                {
                    launched = false;
                }
                else if (Event.Key.Code == sfKeyReturn)
                {
                    if(display_LobbyScreen(Game, config, playFont, port, SERVER, NULL, 0, 4, pseudo))
                    {
                        close = true;
                        launched = false;
                    }
                }
                else if (Event.Key.Code == sfKeyUp && menu_select > 1)
                {
                    sfString_SetColor(serverMenu->texts[menu_select], sfWhite);
                    serverMenu->gui->widget_textbox[menu_select - 1]->active = false;
                    sfString_SetColor(serverMenu->texts[--menu_select], sfRed);
                    serverMenu->gui->widget_textbox[menu_select - 1]->active = true;
                }
                else if (Event.Key.Code == sfKeyDown && menu_select < 3)
                {
                    sfString_SetColor(serverMenu->texts[menu_select], sfWhite);
                    serverMenu->gui->widget_textbox[menu_select - 1]->active = false;
                    sfString_SetColor(serverMenu->texts[++menu_select], sfRed);
                    serverMenu->gui->widget_textbox[menu_select - 1]->active = true;
                }
            }
        }
    }
    while (launched);

    screen_Destroy(serverMenu);
    sfImage_Destroy(textbox_bg);
    sfFont_Destroy(textbox_font);

    return close;
}
