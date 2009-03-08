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
        // Surveillance des évènements
        while (sfRenderWindow_GetEvent(Game, &Event))
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed)
            {
                sfRenderWindow_Close(Game);
                screen_Destroy(Menu);
            }

            // Mettre ici tous les autres events à gérer
        }

        // Vidage de l'écran
        sfRenderWindow_Clear(Game, sfBlack);

        // Dessin du BG
        sfRenderWindow_DrawSprite(Game, Menu->background);

        // Mise à jour de la fenêtre
        sfRenderWindow_Display(Game);
    }
    return EXIT_SUCCESS;
}
