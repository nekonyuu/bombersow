#include <Gui/Gui.h>
#include "BaseSystem/Logging.h"
#include "Game/SettingsScreen.h"
#include "Objects/Screen.h"
#include "GraphicEngine/Draw.h"

_Bool display_Settings(sfRenderWindow* Game, sfImage* BG_image, sfFont* settingsFont)
{
    Screen* Settings = screen_Create();
    sfEvent Event;
    sfImage* bg_textbox = sfImage_CreateFromFile("base/textbox_back.png");
    char player_name[50];
    _Bool launched = true, close = false;

    screen_LoadImage(Settings, BG_image);                        // Chargement de l'arrière-plan
    screen_LoadFont(Settings, settingsFont);                      // Chargement de la police d'écriture
    screen_AddTextbox(Settings, 450, 140, 125, 20, 25, bg_textbox, sfRed, CHAR, player_name, "Pseudo :  ", sfRed, settingsFont, 25);
    screen_LoadText(Settings, GAME_NAME, sfRed, 60, sfStringRegular, 450.0f, 40.0f);

    logging_Info("display_Settings", "Started without error");

    do
    {
        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran

        sfRenderWindow_DrawSprite(Game, Settings->images[0]);   // Dessin du BG
        for (int i = 0; i < Settings->nb_text; i++)
            screen_DrawText(Game, Settings, i);                 // Dessin des textes
        //screen_DrawGui(Game, Settings);                         // Dessin de la GUI

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
