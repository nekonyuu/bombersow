#include <Gui/Gui.h>
#include "BaseSystem/Logging.h"
#include "Game/SettingsScreen.h"
#include "Objects/Screen.h"
#include "GraphicEngine/Draw.h"

_Bool display_Settings(sfRenderWindow* Game, sfImage* BG_image, sfFont* settingsFont)
{
    Screen* Settings = screen_Create();
    sfEvent Event;
    _Bool launched = true, close = false;

    screen_LoadImage(Settings, BG_image);                        // Chargement de l'arri�re-plan
    screen_LoadFont(Settings, settingsFont);                      // Chargement de la police d'�criture
    // Pr�paration des textes


    logging_Info("display_Settings", "Started without error");

    do
    {
        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'�cran

        sfRenderWindow_DrawSprite(Game, Settings->images[0]);    // Dessin du BG

        sfRenderWindow_Display(Game);                           // Mise � jour de la fen�tre

        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des �v�nements
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed)
            {
                screen_Destroy(Credits);
                close = true;                                   // Fermeture du Menu et nettoyage des ressources
                launched = false;
            }
            // Reviens en arri�re
            if (Event.Type == sfEvtKeyPressed && (Event.Key.Code == sfKeyEscape || Event.Key.Code == sfKeyReturn))
            {
                screen_Destroy(Credits);
                launched = false;
            }
        }
    }
    while (launched);

    return close;
}
