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
    Animation *animation = animation_Create(image_animation, 0, 0, 30, 30, 4, 0, -1, 0.1);
    sfEvent Event;
    int menu_select = 1;

    screen_LoadImage(Menu, BG_image);                           // Chargement de l'arrière-plan
    screen_LoadFont(Menu, menuFont);                            // Chargement de la police d'écriture
    screen_LoadText(Menu, GAME_NAME, sfRed);                    // Préparation des textes
    screen_LoadText(Menu, "Jouer", sfRed);
    screen_LoadText(Menu, "Options", sfWhite);
    screen_LoadText(Menu, "Credits", sfWhite);
    screen_LoadText(Menu, "Quitter", sfWhite);
    screen_LoadMusic(Menu, menuMusic, sfTrue);                  // Chargement de la musique
    screen_PlayMusic(Menu);                                     // Lecture

    while (sfRenderWindow_IsOpened(Game))
    {
        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran
        sfRenderWindow_DrawSprite(Game, Menu->images[0]);       // Dessin du BG

        screen_DrawText(Game, Menu, 0, 60, 500.0f, 40.0f);      // Dessin du titre
        screen_DrawText(Game, Menu, 1, 35, 500.0f, 140.0f);
        screen_DrawText(Game, Menu, 2, 35, 500.0f, 190.0f);
        screen_DrawText(Game, Menu, 3, 35, 500.0f, 240.0f);
        screen_DrawText(Game, Menu, 4, 35, 500.0f, 290.0f);

        animation_Draw(animation, Game);                        // Dessin animation test

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre

        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des évènements
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed)
            {
                sfRenderWindow_Close(Game);                     // Fermeture de la fenêtre
                screen_Destroy(Menu);                           // Fermeture du Menu et nettoyage des ressources
            }
            if (Event.Type == sfEvtKeyPressed)
            {
                if (Event.Key.Code == sfKeyEscape)
                {
                    sfRenderWindow_Close(Game);
                    screen_Destroy(Menu);
                }
                else if (Event.Key.Code == sfKeyReturn)
                {
                    switch (menu_select)
                    {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        sfRenderWindow_Close(Game);
                        screen_Destroy(Menu);
                        break;
                    }
                }
                else if (Event.Key.Code == sfKeyUp && menu_select > 1)
                {
                    sfString_SetColor(Menu->texts[menu_select], sfWhite);
                    sfString_SetColor(Menu->texts[--menu_select], sfRed);
                }
                else if (Event.Key.Code == sfKeyDown && menu_select < 4)
                {
                    sfString_SetColor(Menu->texts[menu_select], sfWhite);
                    sfString_SetColor(Menu->texts[++menu_select], sfRed);
                }
            }
        }
    }

    return;
}
