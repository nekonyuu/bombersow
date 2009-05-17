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
#include "GraphicEngine/Draw.h"
#include "Objects/Screen.h"
#include "Objects/GameObjects.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "PhysicsEngine/CollisionSystem.h"
#include "PhysicsEngine/GravitySystem.h"

void display_Menu(sfRenderWindow* Game, Config* config)
{
    Screen* Menu = screen_Create();
    sfMusic *menuMusic = sfMusic_CreateFromFile("sounds/music/ParagonX9 - Metropolis [8-Bit].ogg");
    sfImage *BG_image = sfImage_CreateFromFile("base/images/Menu/menu_bg.png");
    sfFont *menuFont = sfFont_CreateFromFile("base/fonts/ITCKRIST.TTF", 50, NULL);
    sfEvent Event;
    _Bool launched = true;
    int menu_select = 1;

    screen_LoadImage(Menu, BG_image);                           // Chargement de l'arrière-plan
    screen_LoadFont(Menu, menuFont);                            // Chargement de la police d'écriture
    // Préparation des textes
    screen_LoadText(Menu, GAME_NAME, sfRed, 50, sfStringRegular, 450.0f, 40.0f);
    screen_LoadText(Menu, "Jouer", sfRed, 35, sfStringItalic, 450.0f, 140.0f);
    screen_LoadText(Menu, "Options", sfWhite, 35, sfStringItalic, 450.0f, 190.0f);
    screen_LoadText(Menu, "Credits", sfWhite, 35, sfStringItalic, 450.0f, 240.0f);
    screen_LoadText(Menu, "Quitter", sfWhite, 35, sfStringItalic, 450.0f, 290.0f);
    screen_LoadText(Menu, "Test Screen", sfWhite, 35, sfStringItalic, 450.0f, 340.0f);
    screen_LoadMusic(Menu, menuMusic, sfTrue);                  // Chargement de la musique

#ifndef DEBUG_MODE
    if (Menu->music)
        screen_PlayMusic(Menu);                                 // Lecture
#endif

    logging_Info("display_Menu", "Started without error");

    do
    {
        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran

        sfRenderWindow_DrawSprite(Game, Menu->images[0]);       // Dessin du BG

        for (int i = 0; i < Menu->nb_text; i++)
            screen_DrawText(Game, Menu, i);                     // Dessin des textes

        if(config->show_fps)
            logging_FPSShow(Game);

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre

        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des évènements
        {
            switch (Event.Type)
            {
            case sfEvtClosed:                                   // Fermer : Quitter le jeu
                launched = false;
                break;

            case sfEvtKeyPressed:
                switch (Event.Key.Code)
                {
                case sfKeyEscape:                               // Quitte le jeu
                    launched = false;
                    break;

                case sfKeyReturn:
                    switch (menu_select)
                    {
                    case 1:                                     // Affichage du menu de jeu
                        if (display_PlayMenu(Game, config, BG_image, menuFont))
                            launched = false;
                        break;
                    case 2:                                     // Affichage du menu options
                        if (display_Settings(Game, config, BG_image, menuFont))
                            launched = false;
                        break;
                    case 3:                                     // Affichage des credits
                        if (display_Credits(Game, config, BG_image, menuFont))
                            launched = false;
                        break;
                    case 4:                                     // Quitter
                        launched = false;
                        break;
                    case 5:
                        if(display_Playing(Game, config))
                            launched = false;
                        break;
                    }
                    break;

                case sfKeyUp:
                    if (menu_select > 1)
                    {
                        sfString_SetColor(Menu->texts[menu_select], sfWhite);
                        sfString_SetColor(Menu->texts[--menu_select], sfRed);
                    }
                    break;

                case sfKeyDown:
                    if (menu_select < 5)
                    {
                        sfString_SetColor(Menu->texts[menu_select], sfWhite);
                        sfString_SetColor(Menu->texts[++menu_select], sfRed);
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
    while (launched);

    sfFont_Destroy(menuFont);
    sfImage_Destroy(BG_image);
    screen_Destroy(Menu);

    return;
}
