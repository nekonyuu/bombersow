#include "Game/MenuScreen.h"
#include "Game/CreditsScreen.h"
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
    _Bool launched = true;
    int menu_select = 1;

    screen_LoadImage(Menu, BG_image);                           // Chargement de l'arri�re-plan
    screen_LoadFont(Menu, menuFont);                            // Chargement de la police d'�criture
    screen_LoadText(Menu, GAME_NAME, sfRed);                    // Pr�paration des textes
    screen_LoadText(Menu, "Jouer", sfRed);
    screen_LoadText(Menu, "Options", sfWhite);
    screen_LoadText(Menu, "Credits", sfWhite);
    screen_LoadText(Menu, "Quitter", sfWhite);
    screen_LoadMusic(Menu, menuMusic, sfTrue);                  // Chargement de la musique
    screen_PlayMusic(Menu);                                     // Lecture

    while (launched)
    {
        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'�cran

        sfRenderWindow_DrawSprite(Game, Menu->images[0]);       // Dessin du BG
        screen_DrawText(Game, Menu, 0, 60, 450.0f, 40.0f);      // Dessin du titre
        screen_DrawText(Game, Menu, 1, 35, 450.0f, 140.0f);     // Dessin des options
        screen_DrawText(Game, Menu, 2, 35, 450.0f, 190.0f);
        screen_DrawText(Game, Menu, 3, 35, 450.0f, 240.0f);
        screen_DrawText(Game, Menu, 4, 35, 450.0f, 290.0f);
        animation_Draw(animation, Game);                        // Dessin animation test

        sfRenderWindow_Display(Game);                           // Mise � jour de la fen�tre

        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des �v�nements
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed)
            {
                screen_Destroy(Menu);                           // Fermeture du Menu et nettoyage des ressources
                launched = false;
            }

            if (Event.Type == sfEvtKeyPressed)
            {
                if (Event.Key.Code == sfKeyEscape)              // Echap : Quitte le jeu
                {
                    screen_Destroy(Menu);
                    launched = false;
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
                        if (display_Credits())
                            launched = false;
                        break;
                    case 4:                                     // S�lection de l'option Quitter
                        screen_Destroy(Menu);
                        launched = false;
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

    sfImage_Destroy(BG_image);
    sfImage_Destroy(image_animation);
    animation_Destroy(animation);

    return;
}
