#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include "Game/MenuScreen.h"

int main()
{
    sfWindowSettings Settings = {24, 8, 0};
    sfVideoMode Mode = {900, 600, 32};
    sfRenderWindow* Game;
    sfEvent Event;
    sfMusic* menuMusic = NULL;

    // Cr�ation de la fen�tre principale
    Game = sfRenderWindow_Create(Mode, "BomberSow", sfClose, Settings);
    if (!Game)
        return EXIT_FAILURE;

    if (display_Menu(menuMusic) == EXIT_FAILURE)
        return EXIT_FAILURE;

    // D�marrage du jeu
    // armory_Create(armory); // Remplissage du tableau

    while (sfRenderWindow_IsOpened(Game))
    {
        // Surveillance des �v�nements
        while (sfRenderWindow_GetEvent(Game, &Event))
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed)
            {
                sfRenderWindow_Close(Game);
                stopmusic_Menu(menuMusic);
            }

            // Mettre ici tous les autres events � g�rer
        }

        // Vidage de l'�cran
        sfRenderWindow_Clear(Game, sfBlack);

        // Mise � jour de la fen�tre
        sfRenderWindow_Display(Game);
    }

    // Nettoyage des ressources
    sfRenderWindow_Destroy(Game);

    return EXIT_SUCCESS;
}
