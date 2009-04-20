#include "BaseSystem/Logging.h"
#include "Objects/Screen.h"
#include "GraphicEngine/Draw.h"
#include "GraphicEngine/Image.h"
#include "Gui/Gui.h"
#include "Networking/Networking.h"

_Bool display_ServerMenu(sfRenderWindow* Game, sfImage* BG_image, sfFont* playFont)
{
    Screen* serverMenu = screen_Create();
    sfImage* textbox_bg = sfImage_CreateFromFile("base/images/gui/textbox_back.png");
    sfEvent Event;
    _Bool launched = true, close = false;
    int menu_select = 1, port = DEFAULT_PORT;
    char pseudo[20] = "Player", servername[20] = "BomberSow Server";

    screen_LoadImage(serverMenu, BG_image);                             // Chargement de l'arrière-plan
    screen_LoadFont(serverMenu, playFont);                              // Chargement de la police d'écriture
    // Préparation des textes
    screen_LoadText(serverMenu, GAME_NAME, sfRed, 60, sfStringRegular, 450.0f, 40.0f);
    screen_LoadText(serverMenu, "Nom du serveur", sfRed, 15, sfStringRegular, 450.0f, 140.0f);
    screen_LoadText(serverMenu, "Pseudo", sfWhite, 35, sfStringRegular, 450.0f, 190.0f);
    screen_LoadText(serverMenu, "Port", sfWhite, 35, sfStringRegular, 450.0f, 240.0f);
    gui_Add_Textbox(serverMenu->gui, widget_textbox_Create(600, 150, 200, 25, 20, textbox_bg, sfBlack, CHAR, servername, "", sfBlack, playFont, 10));
    gui_Add_Textbox(serverMenu->gui, widget_textbox_Create(600, 200, 200, 25, 20, textbox_bg, sfBlack, CHAR, pseudo, "", sfBlack, playFont, 10));
    gui_Add_Textbox(serverMenu->gui, widget_textbox_Create(600, 250, 100, 25, 5, textbox_bg, sfBlack, INT, &port, "", sfBlack, playFont, 10));
    serverMenu->gui->widget_textbox[0]->active = true;

    logging_Info("display_ServerMenu", "Started without error");

    do
    {
        sfRenderWindow_Clear(Game, sfBlack);                            // Vidage de l'écran

        sfRenderWindow_DrawSprite(Game, serverMenu->images[0]);         // Dessin du BG

        for (int i = 0; i < serverMenu->nb_text; i++)
            screen_DrawText(Game, serverMenu, i);                       // Dessin des textes

        gui_Draw(Game, serverMenu->gui);                                // Dessin de la GUI
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

    return close;
}
