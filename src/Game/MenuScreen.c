#include "BaseSystem/Logging.h"
#include "Game/MenuScreen.h"
#include "Game/PlayScreen.h"
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

    screen_LoadImage(Menu, BG_image);                           // Chargement de l'arrière-plan
    screen_LoadFont(Menu, menuFont);                            // Chargement de la police d'écriture
    // Préparation des textes
    screen_LoadText(Menu, GAME_NAME, sfRed, 60, sfStringRegular, 450.0f, 40.0f);
    screen_LoadText(Menu, "Jouer", sfRed, 35, sfStringItalic, 450.0f, 140.0f);
    screen_LoadText(Menu, "Options", sfWhite, 35, sfStringItalic, 450.0f, 190.0f);
    screen_LoadText(Menu, "Credits", sfWhite, 35, sfStringItalic, 450.0f, 240.0f);
    screen_LoadText(Menu, "Quitter", sfWhite, 35, sfStringItalic, 450.0f, 290.0f);
    screen_LoadMusic(Menu, menuMusic, sfTrue);                  // Chargement de la musique
    if (Menu->music)
        screen_PlayMusic(Menu);                                 // Lecture

    logging_Info("display_Menu", "Started without error");

    do
    {
        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran

        sfRenderWindow_DrawSprite(Game, Menu->images[0]);       // Dessin du BG

        for (int i = 0; i < Menu->nb_text; i++)
            screen_DrawText(Game, Menu, i);                     // Dessin des textes

        animation_Draw(animation, Game);                        // Dessin animation test

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre

        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des évènements
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
                        if (display_PlayMenu(Game, BG_image, menuFont))
                            launched = false;
                        break;
                    case 2:
                        break;
                    case 3:
                        if (display_Credits(Game, BG_image, menuFont))
                            launched = false;
                        break;
                    case 4:                                     // Sélection de l'option Quitter
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
    while (launched);

    sfFont_Destroy(menuFont);
    sfImage_Destroy(BG_image);
    sfImage_Destroy(image_animation);
    animation_Destroy(animation);

    return;
}
