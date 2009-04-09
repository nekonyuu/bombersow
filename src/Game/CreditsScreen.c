#include "BaseSystem/Logging.h"
#include "Game/CreditsScreen.h"
#include "Objects/Screen.h"
#include "GraphicEngine/Draw.h"
#include "Memleak/halloc.h"

_Bool display_Credits(sfRenderWindow* Game, sfImage* BG_image, sfFont* creditsFont)
{
    Screen* Credits = screen_Create();
    sfEvent Event;
    _Bool launched = true, close = false;

    screen_LoadImage(Credits, BG_image);                        // Chargement de l'arrière-plan
    screen_LoadFont(Credits, creditsFont);                      // Chargement de la police d'écriture
    // Préparation des textes
    screen_LoadText(Credits, GAME_NAME, sfRed, 60, sfStringRegular, 450.0f, 40.0f);
    screen_LoadText(Credits, "Auteurs :", sfWhite, 20, sfStringRegular, 450.0f, 140.0f);
    screen_LoadText(Credits, "Raffre Jonathan (Loven x Kotonoha)", sfWhite, 20, sfStringRegular, 450.0f, 190.0f);
    screen_LoadText(Credits, "Netcode, Gestion Objects", sfWhite, 20, sfStringRegular, 450.0f, 220.0f);
    screen_LoadText(Credits, "Brieulle Ludovic", sfWhite, 20, sfStringRegular, 450.0f, 260.0f);
    screen_LoadText(Credits, "Graphisme, Détection clavier", sfWhite, 20, sfStringRegular, 450.0f, 290.0f);
    screen_LoadText(Credits, "Norindr Ananda (kastor)", sfWhite, 20, sfStringRegular, 450.0f, 330.0f);
    screen_LoadText(Credits, "Moteur graphique, physique", sfWhite, 20, sfStringRegular, 450.0f, 360.0f);
    screen_LoadText(Credits, "Ce jeu a été codé en C \"POO\" avec la SFML", sfWhite, 20, sfStringRegular, 450.0f, 430.0f);
    screen_LoadText(Credits, "http://www.sfml-dev.org/", sfWhite, 20, sfStringItalic, 450.0f, 460.0f);

    logging_Info("display_Credits", "Started without error");

    do
    {
        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran

        sfRenderWindow_DrawSprite(Game, Credits->images[0]);    // Dessin du BG

        for (int i = 0; i < Credits->nb_text; i++)              // Dessin des textes
            screen_DrawText(Game, Credits, i);

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre

        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des évènements
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed)
            {
                close = true;                                   // Fermeture du Menu et nettoyage des ressources
                launched = false;
            }
            // Reviens en arrière
            if (Event.Type == sfEvtKeyPressed && (Event.Key.Code == sfKeyEscape || Event.Key.Code == sfKeyReturn))
                launched = false;
        }
    }
    while (launched);

    screen_Destroy(Credits);

    return close;
}
