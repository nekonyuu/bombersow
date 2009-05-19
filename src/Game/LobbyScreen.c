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
#include "Networking/Networking.h"

_Bool display_LobbyScreen(sfRenderWindow* Game, Config* config, sfFont* font, unsigned int port, link_t link_type, char* ip, unsigned int map_id, unsigned int nb_players, char* player_name)
{
    Screen* lobby_view = screen_Create();
    Map* map = NULL;
    PlayersList* players_display = NULL;
    ClientData* client_data = NULL;
    sfThread* server_thread = NULL, *client_thread = NULL;
    sfString* text_display = sfString_Create();
    sfEvent Event;
    _Bool launched = true, close = false;

    server_creation = sfMutex_Create();

    // Squelette de l'écran Lobby
    screen_LoadFont(lobby_view, font);
    screen_LoadText(lobby_view, "Joueurs connectés", sfRed, 18, sfStringRegular, 40.f, 25.f);

    // Ecran d'attente
    sfString_SetFont(text_display, font);
    sfString_SetText(text_display, "Connexion en cours...");
    sfString_SetSize(text_display, 30);
    sfString_SetX(text_display, 0);
    sfString_SetY(text_display, 0);

    sfRenderWindow_Clear(Game, sfBlack);
    sfRenderWindow_DrawString(Game, text_display);
    sfRenderWindow_Display(Game);

    // Si mode Server
    if (link_type == SERVER)
    {
        client_data = clientdata_Create(player_name, "127.0.0.1", port, config);

        map = map_Create(map_id, nb_players, config);
        map_SetGamePort(map, port);

        server_thread = sfThread_Create(&server_Main, map);
        client_thread = sfThread_Create(&client_Main, client_data);
        sfThread_Launch(server_thread);
        sfSleep(0.5f);
        sfThread_Launch(client_thread);
        sfSleep(0.5f);
    }
    else if (link_type == CLIENT)       // Sinon si mode Client
    {
        client_data = clientdata_Create(player_name, ip, port, config);
        client_thread = sfThread_Create(&client_Main, client_data);
        sfThread_Launch(client_thread);
        sfSleep(0.5f);
    }

    sfMutex_Lock(server_creation);

    while(!client_connected)
        sfSleep(0.1f);

    players_display = playerslist_Create(client_data->map, font, sfWhite, 12, sfStringItalic, 40.f, 60.f);

    do
    {
        sfRenderWindow_Clear(Game, sfBlack);

        // Actualisation des joueurs connectés
        playerslist_Update(players_display, client_data->map);

        playerslist_Draw(players_display, Game);

        for (int i = 0; i < lobby_view->nb_text; i++)
            screen_DrawText(Game, lobby_view, i);                       // Dessin des textes

        if(config->show_fps)
            logging_FPSShow(Game);

        sfRenderWindow_Display(Game);

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
                gui_Click(lobby_view->gui, Event.MouseButton.X, Event.MouseButton.Y);
            }

            if (Event.Type == sfEvtTextEntered)                         // Entrée de texte
            {
                gui_TextEntered(lobby_view->gui, Event.Text.Unicode);
            }

            if (Event.Type == sfEvtKeyPressed)
            {
                if (Event.Key.Code == sfKeyEscape)                      // Echap : Reviens en arrière
                {
                    launched = false;
                }
                else if (Event.Key.Code == sfKeyReturn)
                {

                }
            }
        }
    }
    while (launched && !client_data->server_close);

    server_started = false;
    client_connected = false;

    sfThread_Wait(client_thread);
    sfThread_Destroy(client_thread);

    if(server_thread)
    {
        sfThread_Wait(server_thread);
        sfThread_Destroy(server_thread);
    }

    sfMutex_Destroy(server_creation);

    clientdata_Destroy(client_data);
    playerslist_Destroy(players_display);
    map_Destroy(map);

    screen_Destroy(lobby_view);

    return close;
}
