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

bool display_Credits(sfRenderWindow* Game, Config* config, sfImage* BG_image, sfFont* creditsFont)
{
    Screen* Credits = screen_Create();
    sfEvent Event;
    bool launched = true, close = false;

    screen_LoadImage(Credits, BG_image);                        // Chargement de l'arrière-plan
    screen_LoadFont(Credits, creditsFont);                      // Chargement de la police d'écriture
    // Préparation des textes
    screen_LoadText(Credits, GAME_NAME, sfRed, 50, sfStringRegular, 450.0f, 40.0f);
    screen_LoadText(Credits, "Auteurs :", sfWhite, 20, sfStringRegular, 450.0f, 140.0f);
    screen_LoadText(Credits, "Raffre Jonathan (Loven x Kotonoha)", sfWhite, 20, sfStringRegular, 450.0f, 190.0f);
    screen_LoadText(Credits, "Netcode, Gestion Objects", sfWhite, 20, sfStringRegular, 450.0f, 220.0f);
    screen_LoadText(Credits, "Brieulle Ludovic (Klice)", sfWhite, 20, sfStringRegular, 450.0f, 260.0f);
    screen_LoadText(Credits, "Graphisme, Gestion clavier, Gameplay", sfWhite, 20, sfStringRegular, 450.0f, 290.0f);
    screen_LoadText(Credits, "Norindr Ananda (Kastor)", sfWhite, 20, sfStringRegular, 450.0f, 330.0f);
    screen_LoadText(Credits, "Moteur graphique, physique", sfWhite, 20, sfStringRegular, 450.0f, 360.0f);
    screen_LoadText(Credits, "Ce jeu a été codé en C \"POO\" avec la SFML", sfWhite, 20, sfStringRegular, 450.0f, 430.0f);
    screen_LoadText(Credits, "http://www.sfml-dev.org/", sfWhite, 20, sfStringItalic, 450.0f, 460.0f);
    screen_LoadText(Credits, "Musiques par ParagonX9", sfWhite, 20, sfStringRegular, 450.0f, 520.0f);
    screen_LoadText(Credits, "Licence Creative Commons BY-NC-SA", sfWhite, 20, sfStringRegular, 450.0f, 550.0f);

    logging_Info("display_Credits", "Started without error");

    do
    {
        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran

        sfRenderWindow_DrawSprite(Game, Credits->images[0]);    // Dessin du BG

        for (int i = 0; i < Credits->nb_text; i++)              // Dessin des textes
            screen_DrawText(Game, Credits, i);

        if(config->show_fps)
            logging_FPSShow(Game);

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre

        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des évènements
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed)
            {
                close = true;                                   // Fermeture du Menu et nettoyage des ressources
                launched = false;
            }
            // Reviens en arrière
            if (Event.Type == sfEvtKeyPressed && (Event.Key.Code == sfKeyEscape || Event.Key.Code == sfKeyReturn))
                launched = false;
        }
    }
    while (launched);

    screen_Destroy(Credits);

    return close;
}
