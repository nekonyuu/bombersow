#include "BaseSystem/Logging.h"
#include "Game/MenuScreen.h"
#include "Game/CreditsScreen.h"
#include "Objects/Screen.h"
#include "GraphicEngine/Draw.h"
#include "GraphicEngine/Image.h"
#include "Objects/GameObjects.h"

_Bool display_PlayMenu(sfRenderWindow* Game, sfImage* BG_image, sfFont* playFont)
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
                    case 1:
                        break;
                    case 2:
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
