#include "Game/MenuScreen.h"
#include "Objects/Screen.h"
#include "GraphicEngine/Draw.h"
#include "GraphicEngine/Image.h"
#include "Objects/Map.h"

void display_Menu(sfRenderWindow* Game)
{
    Screen* Menu = screen_Create();
    sfMusic *menuMusic = sfMusic_CreateFromFile("sounds/music/ParagonX9 - Metropolis [8-Bit].ogg");
    sfImage *BG_image = sfImage_CreateFromFile("base/images/Menu/menu_bg.png");
    sfImage *image_animation = sfImage_CreateFromFile("base/images/animation.png"); // Test
    sfFont *menuFont = sfFont_CreateFromFile("base/fonts/ITCKRIST.TTF", 50, NULL);
    sfColor title_color = sfColor_FromRGB(225, 0, 0);
    Animation *animation = animation_Create(image_animation, 0, 0, 30, 30, 4, 0, -1, 0.1);
    sfEvent Event;
    int menu_select = 0;

    // Chargement de l'image d'arrière plan, de la musique, et lecture
    screen_LoadImage(Menu, BG_image);
    screen_LoadFont(Menu, menuFont);
    screen_LoadText(Menu, GAME_NAME, title_color);
    screen_LoadMusic(Menu, menuMusic, sfTrue);
    screen_PlayMusic(Menu);

    while (sfRenderWindow_IsOpened(Game))
    {
        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des évènements
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed)
            {
                sfRenderWindow_Close(Game);
                screen_Destroy(Menu);                           // Fermeture du Menu et nettoyage des ressources
            }
            if (Event.Type == sfEvtKeyPressed)
            {
                if (Event.Key.Code == sfKeyEscape)
                {
                    sfRenderWindow_Close(Game);
                    screen_Destroy(Menu);                       // Fermeture du Menu et nettoyage des ressources
                }
                else if (Event.Key.Code == sfKeyReturn)
                {
                    switch (menu_select)
                    {
                    case 0:
                        break;
                    case 1:
                        break;
                    case 2:
                        break;
                    }
                }
                else if (Event.Key.Code == sfKeyUp && menu_select > 0)
                    menu_select--;
                else if (Event.Key.Code == sfKeyDown && menu_select < 2)
                    menu_select++;
            }

            // Mettre ici tous les autres events à gérer
        }

        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran
        sfRenderWindow_DrawSprite(Game, Menu->images[0]);       // Dessin du BG

        screen_DrawText(Game, Menu, 0, 50, 500.0f, 40.0f);      // Dessin du titre
        animation_Draw(animation, Game);                        // Dessin animation test

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre
    }

    return;
}
