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

#include <Gui/Gui.h>
#include "BaseSystem/Logging.h"
#include "Game/GameScreens.h"
#include "Objects/Screen.h"
#include "GraphicEngine/Draw.h"

bool display_Settings(sfRenderWindow* Game, Config* config, sfImage* BG_image, sfFont* settingsFont)
{
    Screen* Settings = screen_Create();
    sfEvent Event;
    //sfImage* bg_textbox = sfImage_CreateFromFile("base/images/gui/textbox_back.png");
    //char player_name[50];
    bool launched = true, close = false;

    screen_LoadImage(Settings, BG_image);                       // Chargement de l'arrière-plan
    screen_LoadFont(Settings, settingsFont);                    // Chargement de la police d'écriture
    //screen_AddTextbox(Settings, 450, 140, 125, 20, 25, bg_textbox, sfRed, CHAR, player_name, "Pseudo :  ", sfRed, settingsFont, 25);
    screen_LoadText(Settings, GAME_NAME, sfRed, 50, sfStringRegular, 450.0f, 40.0f);

    logging_Info("display_Settings", "Started without error");

    do
    {
        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran

        sfRenderWindow_DrawSprite(Game, Settings->images[0]);   // Dessin du BG
        for (int i = 0; i < Settings->nb_text; i++)
            screen_DrawText(Game, Settings, i);                 // Dessin des textes
        //screen_DrawGui(Game, Settings);                         // Dessin de la GUI

        if(config->show_fps)
            logging_FPSShow(Game);

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre

        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des évènements
        {
            switch(Event.Type)
            {
                case sfEvtClosed:
                    close = true;                               // Fermeture du Menu et nettoyage des ressources
                    launched = false;
                    break;

                case sfEvtKeyPressed:
                    switch(Event.Key.Code)
                    {
                    case sfKeyEscape:                           // Retour en arrière
                        launched = false;
                        break;

                    default:
                        break;
                    }
                    break;

                case sfEvtMouseButtonPressed:                   // Clic sur un élément de la GUI
                    gui_Click(Settings->gui, Event.MouseButton.X, Event.MouseButton.Y);
                    break;

                case sfEvtTextEntered:                          // Entrée de texte
                    gui_TextEntered(Settings->gui, Event.Text.Unicode);
                    break;

                default:
                    break;
            }
        }
    }
    while (launched);

    screen_Destroy(Settings);

    return close;
}
