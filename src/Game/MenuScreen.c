#include "Game/MenuScreen.h"
#include "Objects/Screen.h"

int display_Menu(sfRenderWindow* Game)
{
    Screen* Menu = screen_Create();
    sfMusic *menuMusic = sfMusic_CreateFromFile("sounds/music/ParagonX9 - Defcon Zero.ogg");
    sfImage *BG_image = sfImage_CreateFromFile("base/images/coming_soon.jpg");
    sfEvent Event;

    screen_LoadMusic(Menu, menuMusic, sfTrue);
    screen_LoadBG(Menu, BG_image);

    while (sfRenderWindow_IsOpened(Game))
    {
        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des �v�nements
        {
            if (Event.Type == sfEvtClosed)                      // Fermer : Quitter le jeu
            {
                sfRenderWindow_Close(Game);
                screen_Destroy(Menu);                           // Fermeture du Menu et nettoyage des ressources
            }

            // Mettre ici tous les autres events � g�rer
        }

        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'�cran
        sfRenderWindow_DrawSprite(Game, Menu->background);      // Dessin du BG
        sfRenderWindow_Display(Game);                           // Mise � jour de la fen�tre
    }
    return EXIT_SUCCESS;
}
