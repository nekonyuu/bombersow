#include "Game/MenuScreen.h"
#include "Objects/Screen.h"

void display_Menu(sfRenderWindow* Game)
{
    Screen* Menu = screen_Create();
    sfMusic *menuMusic = sfMusic_CreateFromFile("sounds/music/ParagonX9 - Defcon Zero.ogg");
    sfImage *BG_image = sfImage_CreateFromFile("base/images/coming_soon.jpg");
    sfEvent Event;

    // Chargement de l'image d'arri�re plan, de la musique, et lecture
    screen_LoadBG(Menu, BG_image);
    screen_LoadMusic(Menu, menuMusic, sfTrue);
    screen_PlayMusic(Menu);

    while (sfRenderWindow_IsOpened(Game))
    {
        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des �v�nements
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed
            || (Event.Type == sfEvtKeyPressed && Event.Key.Code == sfKeyEscape))
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
    return;
}
