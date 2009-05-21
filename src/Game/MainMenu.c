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

#include "BaseSystem/Config.h"
#include "BaseSystem/Logging.h"
#include "Game/GameScreens.h"
#include "Networking/Networking.h"
#include "Objects/Screen.h"

#define NB_SCREENS 5
enum { MAIN_MENU_SCR, PLAY_SCR, SERVER_SCR, CLIENT_SCR, CREDITS_SCR };

int game_MainMenu(sfRenderWindow* Game, Config* config)
{
    Screen** Screens_List = NULL;
    Screen* Current_Screen = NULL;
    // Préparation des ressources
    sfImage* textbox_bg_white = sfImage_CreateFromFile("base/images/gui/textbox_back.png");
    sfImage* textbox_bg_black = sfImage_CreateFromFile("base/images/gui/textbox_back_black.png");

    sfEvent Event;
    bool close = false;
    int menu_select = 1;
    int error_code = NO_ERROR;

    char pseudo[20] = "Player", servername[20] = "Bombersow Server", ip[15] = "127.0.0.1";
    int port = DEFAULT_PORT;

    assert(Screens_List = (Screen**) malloc(NB_SCREENS * sizeof(Screen*)));

    /* ------------------- PREPARATION DES SCREENS -------------------- */

    // Menu Principal
    Screens_List[MAIN_MENU_SCR] = screen_Create();
    screen_LoadFont(Screens_List[MAIN_MENU_SCR], OPT_FONT, "base/fonts/ITCKRIST.TTF");
    screen_LoadImage(Screens_List[MAIN_MENU_SCR], "base/images/Menu/menu_bg.png");
    screen_LoadMusic(Screens_List[MAIN_MENU_SCR], "sounds/music/ParagonX9 - Metropolis [8-Bit].ogg", sfTrue);
    screen_LoadText(Screens_List[MAIN_MENU_SCR], GAME_NAME, sfRed, 50, sfStringRegular, 450.0f, 40.0f);
    screen_LoadText(Screens_List[MAIN_MENU_SCR], "Jouer", sfRed, 35, sfStringItalic, 450.0f, 140.0f);
    screen_LoadText(Screens_List[MAIN_MENU_SCR], "Options", sfWhite, 35, sfStringItalic, 450.0f, 190.0f);
    screen_LoadText(Screens_List[MAIN_MENU_SCR], "Credits", sfWhite, 35, sfStringItalic, 450.0f, 240.0f);
    screen_LoadText(Screens_List[MAIN_MENU_SCR], "Quitter", sfWhite, 35, sfStringItalic, 450.0f, 290.0f);
    screen_LoadText(Screens_List[MAIN_MENU_SCR], "Test Screen", sfWhite, 35, sfStringItalic, 450.0f, 340.0f);
    screen_SetMenuInterval(Screens_List[MAIN_MENU_SCR], 1, 5);

    // Menu de jeu
    Screens_List[PLAY_SCR] = screen_Create();
    screen_LoadFont(Screens_List[PLAY_SCR], OPT_FONT, "base/fonts/ITCKRIST.TTF");
    screen_LoadImage(Screens_List[PLAY_SCR], "base/images/Menu/menu_bg.png");
    screen_LoadText(Screens_List[PLAY_SCR], GAME_NAME, sfRed, 50, sfStringRegular, 450.0f, 40.0f);
    screen_LoadText(Screens_List[PLAY_SCR], "Rejoindre une partie", sfRed, 35, sfStringItalic, 450.0f, 140.0f);
    screen_LoadText(Screens_List[PLAY_SCR], "Créer un serveur", sfWhite, 35, sfStringItalic, 450.0f, 190.0f);
    screen_LoadText(Screens_List[PLAY_SCR], "Retour", sfWhite, 35, sfStringItalic, 450.0f, 240.0f);
    screen_SetMenuInterval(Screens_List[PLAY_SCR], 1, 3);

    // Menu Server
    Screens_List[SERVER_SCR] = screen_Create();
    screen_LoadFont(Screens_List[SERVER_SCR], OPT_FONT, "base/fonts/ITCKRIST.TTF");
    screen_LoadFont(Screens_List[SERVER_SCR], GUI_FONT, "base/fonts/friday13v12.ttf");
    screen_LoadFont(Screens_List[SERVER_SCR], ALT_GUI_FONT, "base/fonts/ITCKRIST.TTF");
    screen_LoadImage(Screens_List[SERVER_SCR], "base/images/Menu/menu_bg.png");
    screen_LoadText(Screens_List[SERVER_SCR], GAME_NAME, sfRed, 50, sfStringRegular, 450.0f, 40.0f);
    screen_LoadText(Screens_List[SERVER_SCR], "Nom du serveur", sfRed, 20, sfStringRegular, 450.0f, 250.0f);
    screen_LoadText(Screens_List[SERVER_SCR], "Pseudo", sfWhite, 20, sfStringRegular, 450.0f, 300.0f);
    screen_LoadText(Screens_List[SERVER_SCR], "Port", sfWhite, 20, sfStringRegular, 450.0f, 350.0f);
    screen_LoadText(Screens_List[SERVER_SCR], "Création du serveur", sfWhite, 35, sfStringRegular, 450.0f, 120.0f);
    screen_AddTextbox(Screens_List[SERVER_SCR], 630, 250, 175, 20, 20, textbox_bg_white, sfBlack, CHAR, servername, sfBlack, "", sfBlack, 10);
    screen_AddTextbox(Screens_List[SERVER_SCR], 630, 300, 175, 20, 20, textbox_bg_white, sfBlack, CHAR, pseudo, sfBlack, "", sfBlack, 10);
    screen_AddTextbox(Screens_List[SERVER_SCR], 630, 350, 75, 20, 5, textbox_bg_white, sfBlack, INT, &port, sfBlack, "", sfBlack, 10);
    screen_SetMenuInterval(Screens_List[SERVER_SCR], 1, 3);
    Screens_List[SERVER_SCR]->gui->widget_textbox[0]->active = true;

    // Menu Client
    Screens_List[CLIENT_SCR] = screen_Create();
    screen_LoadFont(Screens_List[CLIENT_SCR], OPT_FONT, "base/fonts/ITCKRIST.TTF");
    screen_LoadFont(Screens_List[CLIENT_SCR], GUI_FONT, "base/fonts/friday13v12.ttf");
    screen_LoadFont(Screens_List[CLIENT_SCR], ALT_GUI_FONT, "base/fonts/ITCKRIST.TTF");
    screen_LoadImage(Screens_List[CLIENT_SCR], "base/images/Menu/menu_bg.png");
    screen_LoadText(Screens_List[CLIENT_SCR], GAME_NAME, sfRed, 50, sfStringRegular, 450.0f, 40.0f);
    screen_LoadText(Screens_List[CLIENT_SCR], "Pseudo", sfRed, 20, sfStringRegular, 450.0f, 250.0f);
    screen_LoadText(Screens_List[CLIENT_SCR], "IP", sfWhite, 20, sfStringRegular, 450.0f, 300.0f);
    screen_LoadText(Screens_List[CLIENT_SCR], "Port", sfWhite, 20, sfStringRegular, 450.0f, 350.0f);
    screen_LoadText(Screens_List[CLIENT_SCR], "Connexion IP", sfWhite, 35, sfStringRegular, 450.0f, 120.0f);
    screen_AddTextbox(Screens_List[CLIENT_SCR], 630, 250, 175, 20, 20, textbox_bg_white, sfBlack, CHAR, pseudo, sfBlack, "", sfBlack, 10);
    screen_AddTextbox(Screens_List[CLIENT_SCR], 630, 300, 175, 20, 15, textbox_bg_white, sfBlack, CHAR, ip, sfBlack, "", sfBlack, 10);
    screen_AddTextbox(Screens_List[CLIENT_SCR], 630, 350, 75, 20, 5, textbox_bg_white, sfBlack, INT, &port, sfBlack, "", sfBlack, 10);
    screen_SetMenuInterval(Screens_List[CLIENT_SCR], 1, 3);
    Screens_List[CLIENT_SCR]->gui->widget_textbox[0]->active = true;

    // Credits
    Screens_List[CREDITS_SCR] = screen_Create();
    screen_LoadFont(Screens_List[CREDITS_SCR], OPT_FONT, "base/fonts/ITCKRIST.TTF");
    screen_LoadImage(Screens_List[CREDITS_SCR], "base/images/Menu/menu_bg.png");
    screen_LoadText(Screens_List[CREDITS_SCR], GAME_NAME, sfRed, 50, sfStringRegular, 450.0f, 40.0f);
    screen_LoadText(Screens_List[CREDITS_SCR], "Auteurs :", sfWhite, 20, sfStringRegular, 450.0f, 140.0f);
    screen_LoadText(Screens_List[CREDITS_SCR], "Raffre Jonathan (Loven x Kotonoha)", sfWhite, 20, sfStringRegular, 450.0f, 190.0f);
    screen_LoadText(Screens_List[CREDITS_SCR], "Netcode, Gestion Objects", sfWhite, 20, sfStringRegular, 450.0f, 220.0f);
    screen_LoadText(Screens_List[CREDITS_SCR], "Brieulle Ludovic (Klice)", sfWhite, 20, sfStringRegular, 450.0f, 260.0f);
    screen_LoadText(Screens_List[CREDITS_SCR], "Graphisme, Gestion clavier, Gameplay", sfWhite, 20, sfStringRegular, 450.0f, 290.0f);
    screen_LoadText(Screens_List[CREDITS_SCR], "Norindr Ananda (Kastor)", sfWhite, 20, sfStringRegular, 450.0f, 330.0f);
    screen_LoadText(Screens_List[CREDITS_SCR], "Moteur graphique, physique", sfWhite, 20, sfStringRegular, 450.0f, 360.0f);
    screen_LoadText(Screens_List[CREDITS_SCR], "Ce jeu a été codé en C \"POO\" avec la SFML", sfWhite, 20, sfStringRegular, 450.0f, 430.0f);
    screen_LoadText(Screens_List[CREDITS_SCR], "http://www.sfml-dev.org/", sfWhite, 20, sfStringItalic, 450.0f, 460.0f);
    screen_LoadText(Screens_List[CREDITS_SCR], "Musiques par ParagonX9", sfWhite, 20, sfStringRegular, 450.0f, 520.0f);
    screen_LoadText(Screens_List[CREDITS_SCR], "Licence Creative Commons BY-NC-SA", sfWhite, 20, sfStringRegular, 450.0f, 550.0f);

    /* ------------------------------ END ----------------------------- */

    Current_Screen = Screens_List[MAIN_MENU_SCR];

#ifndef DEBUG_MODE
    screen_PlayMusic(Screens_List[0]);                          // Lecture
#endif

    logging_Info("game_MainMenu", "Started without error");

    do
    {
        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran

        screen_Draw(Current_Screen, Game);

        if(config->show_fps)
            logging_FPSShow(Game);

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre

        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des évènements
        {
            switch (Event.Type)
            {
            case sfEvtClosed:                                   // Fermer : Quitter le jeu
                close = true;
                break;

            case sfEvtKeyPressed:
                switch (Event.Key.Code)
                {
                case sfKeyEscape:                               // Quitte le jeu
                    if(Current_Screen == Screens_List[MAIN_MENU_SCR])
                        close = true;
                    else if(Current_Screen == Screens_List[CLIENT_SCR] || Current_Screen == Screens_List[SERVER_SCR])
                    {
                        Current_Screen = Screens_List[PLAY_SCR];
                        screen_HightlightText(Current_Screen, 1, sfRed);
                        menu_select = 1;
                    }
                    else
                    {
                        Current_Screen = Screens_List[MAIN_MENU_SCR];
                        screen_HightlightText(Current_Screen, 1, sfRed);
                        menu_select = 1;
                    }
                    break;

                case sfKeyReturn:
                    switch (menu_select)
                    {
                    case 1:
                        if(Current_Screen == Screens_List[MAIN_MENU_SCR])
                        {
                            Current_Screen = Screens_List[PLAY_SCR];
                            screen_HightlightText(Current_Screen, 1, sfRed);
                            menu_select = 1;
                        }
                        else if(Current_Screen == Screens_List[PLAY_SCR])
                        {
                            Current_Screen = Screens_List[CLIENT_SCR];
                            screen_HightlightText(Current_Screen, 1, sfRed);
                            menu_select = 1;
                        }
                        break;
                    case 2:
                        if(Current_Screen == Screens_List[MAIN_MENU_SCR])
                            break;
                        else if(Current_Screen == Screens_List[PLAY_SCR])
                        {
                            Current_Screen = Screens_List[SERVER_SCR];
                            screen_HightlightText(Current_Screen, 1, sfRed);
                            menu_select = 1;
                        }

                        break;
                    case 3:
                        if(Current_Screen == Screens_List[MAIN_MENU_SCR])
                        {
                            Current_Screen = Screens_List[CREDITS_SCR];
                            screen_HightlightText(Current_Screen, 1, sfRed);
                            menu_select = 1;
                        }
                        else if(Current_Screen == Screens_List[PLAY_SCR])
                        {
                            Current_Screen = Screens_List[MAIN_MENU_SCR];
                            screen_HightlightText(Current_Screen, 1, sfRed);
                            menu_select = 1;
                        }
                        break;
                    case 4:
                        if(Current_Screen == Screens_List[MAIN_MENU_SCR])
                            close = true;
                        break;
                    case 5:
                        if(Current_Screen == Screens_List[MAIN_MENU_SCR])
                            display_Playing(Game, config);
                        break;
                    }
                    break;

                case sfKeyUp:
                    if (menu_select > Current_Screen->min_menu)
                    {
                        sfString_SetColor(Current_Screen->texts[menu_select], sfWhite);
                        if(Current_Screen == Screens_List[SERVER_SCR] || Current_Screen == Screens_List[CLIENT_SCR])
                            screen_SetInactiveTextbox(Current_Screen, menu_select - 1);

                        sfString_SetColor(Current_Screen->texts[--menu_select], sfRed);
                        if(Current_Screen == Screens_List[SERVER_SCR] || Current_Screen == Screens_List[CLIENT_SCR])
                            screen_SetActiveTextbox(Current_Screen, menu_select - 1);
                    }
                    break;

                case sfKeyDown:
                    if (menu_select < Current_Screen->max_menu)
                    {
                        sfString_SetColor(Current_Screen->texts[menu_select], sfWhite);
                        if(Current_Screen == Screens_List[SERVER_SCR] || Current_Screen == Screens_List[CLIENT_SCR])
                            screen_SetInactiveTextbox(Current_Screen, menu_select - 1);

                        sfString_SetColor(Current_Screen->texts[++menu_select], sfRed);
                        if(Current_Screen == Screens_List[SERVER_SCR] || Current_Screen == Screens_List[CLIENT_SCR])
                            screen_SetActiveTextbox(Current_Screen, menu_select - 1);
                    }
                    break;

                default:
                    break;
                }
            default:
                break;
            }
        }
    }
    while (!close);

    for (int i = 0; i < NB_SCREENS; i++)
        screen_Destroy(Screens_List[i]);

    free_secure(Screens_List);

    sfImage_Destroy(textbox_bg_black);
    sfImage_Destroy(textbox_bg_white);

    return error_code;
}
