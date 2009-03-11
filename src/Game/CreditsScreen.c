#include "Game/CreditsScreen.h"
#include "Objects/Screen.h"
#include "GraphicEngine/Draw.h"

_Bool display_Credits(sfRenderWindow* Game)
{
    Screen* Credits = screen_Create();
    sfImage *BG_image = sfImage_CreateFromFile("base/images/Menu/menu_bg.png");
    sfFont *creditsFont = sfFont_CreateFromFile("base/fonts/ITCKRIST.TTF", 50, NULL);
    sfEvent Event;
    _Bool launched = true, close = false;

    screen_LoadImage(Credits, BG_image);                        // Chargement de l'arrière-plan
    screen_LoadFont(Credits, creditsFont);                      // Chargement de la police d'écriture
    screen_LoadText(Credits, GAME_NAME, sfRed);                 // Préparation des textes
    screen_LoadText(Credits, "Auteurs :", sfWhite);
    screen_LoadText(Credits, "Raffre Jonathan (Loven x Kotonoha)", sfWhite);
    screen_LoadText(Credits, "Brieulle Ludovic", sfWhite);
    screen_LoadText(Credits, "Norindr Ananda (kastor)", sfWhite);
    screen_LoadText(Credits, "Ce jeu a été codé avec la SFML", sfWhite);
    screen_LoadText(Credits, "http://www.sfml-dev.org/", sfWhite);
    screen_LoadText(Credits, "Appuyez sur Echap pour revenir en arrière", sfWhite);

    while (launched)
    {
        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran

        sfRenderWindow_DrawSprite(Game, Credits->images[0]);    // Dessin du BG
        screen_DrawText(Game, Credits, 0, 60, 450.0f, 40.0f);   // Dessin du titre
        screen_DrawText(Game, Credits, 1, 20, 450.0f, 140.0f);  // Dessin des credits
        screen_DrawText(Game, Credits, 2, 20, 450.0f, 180.0f);
        screen_DrawText(Game, Credits, 3, 20, 450.0f, 220.0f);
        screen_DrawText(Game, Credits, 4, 20, 450.0f, 260.0f);
        screen_DrawText(Game, Credits, 5, 20, 450.0f, 320.0f);
        screen_DrawText(Game, Credits, 6, 20, 450.0f, 360.0f);
        screen_DrawText(Game, Credits, 7, 20, 450.0f, 420.0f);

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre

        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des évènements
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed)
            {
                screen_Destroy(Credits);
                close = true;                         // Fermeture du Menu et nettoyage des ressources
                launched = false;
            }

            if (Event.Type == sfEvtKeyPressed && Event.Key.Code == sfKeyEscape) // Reviens en arrière
            {
                    screen_Destroy(Credits);
                    launched = false;
            }
        }
    }

    sfImage_Destroy(BG_image);

    return close;
}
