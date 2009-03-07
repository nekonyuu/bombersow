#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include "Game/MenuScreen.h"

int main()
{
    sfWindowSettings Settings = {24, 8, 0};
    sfVideoMode Mode = {900, 600, 32};
    sfRenderWindow* Game;
    sfEvent Event;

    // Création de la fenêtre principale
    Game = sfRenderWindow_Create(Mode, "BomberSow", sfClose, Settings);
    if (!Game)
        return EXIT_FAILURE;

    if (display_Menu() == EXIT_FAILURE)
        return EXIT_FAILURE;

    // Démarrage du jeu
    while (sfRenderWindow_IsOpened(Game))
    {
        // Surveillance des évènements
        while (sfRenderWindow_GetEvent(Game, &Event))
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed)
                sfRenderWindow_Close(Game);

            // Mettre ici tous les autres events à gérer
        }

        // Vidage de l'écran
        sfRenderWindow_Clear(Game, sfBlack);

        // Mise à jour de la fenêtre
        sfRenderWindow_Display(Game);
    }

    // Nettoyage des ressources
    sfRenderWindow_Destroy(Game);

    return EXIT_SUCCESS;
}
