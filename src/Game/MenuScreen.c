#include "Game/MenuScreen.h"
#include "Objects/Screen.h"
#include "GraphicEngine/Image.h"

void display_Menu(sfRenderWindow* Game)
{
    Screen* Menu = screen_Create();
    sfMusic *menuMusic = sfMusic_CreateFromFile("sounds/music/ParagonX9 - Metropolis [8-Bit].ogg");
    sfImage *BG_image = sfImage_CreateFromFile("base/images/coming_soon.jpg");

    sfImage *image_animation = sfImage_CreateFromFile("base/images/animation.png");
    Animation *animation = animation_Create(image_animation, 0, 0, 30, 30, 4, 0, BOUCLE, 100);

    sfEvent Event;

    // Chargement de l'image d'arrière plan, de la musique, et lecture
    screen_LoadBG(Menu, BG_image);
    screen_LoadMusic(Menu, menuMusic, sfTrue);
    screen_PlayMusic(Menu);

    while (sfRenderWindow_IsOpened(Game))
    {
        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des évènements
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed
            || (Event.Type == sfEvtKeyPressed && Event.Key.Code == sfKeyEscape))
            {
                sfRenderWindow_Close(Game);
                screen_Destroy(Menu);                           // Fermeture du Menu et nettoyage des ressources
            }

            // Mettre ici tous les autres events à gérer
        }

        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran
        sfRenderWindow_DrawSprite(Game, Menu->background);      // Dessin du BG
        animation_Draw(animation, Game);
        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre
    }
    return;
}
