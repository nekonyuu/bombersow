#include "SFML/Graphics.h"
#include "SFML/Audio.h"
#include "Screen/Objects_Screen.h"
#include "File/file.h"

int main()
{
    sfWindowSettings Settings = {24, 8, 0};
    sfVideoMode Mode = {1100, 800, 32};
    sfRenderWindow* Game;

    sfEvent Event;

    Image *image_object = image_Create();
    dossier_Read_Image(image_object, "base/images/");

    // Création de la fenêtre principale
    Game = sfRenderWindow_Create(Mode, "BomberSowEditor", sfClose, Settings);

    sfImage *fond_menu = sfImage_CreateFromFile("base/fond_object.png");
    Object_Screen* object_screen = object_screen_Create(Game, fond_menu, 0, 600, 1100, 200);
    object_screen_Load_Object(object_screen, image_object);

    if (!Game)
        return EXIT_FAILURE;

    sfRenderWindow_SetFramerateLimit(Game, 60);
    // Démarrage du jeu
    while (sfRenderWindow_IsOpened(Game))
    {
        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des évènements
        {
            // Fermer : Quitter le jeu
            if (Event.Type == sfEvtClosed
            || (Event.Type == sfEvtKeyPressed && Event.Key.Code == sfKeyEscape))
            {
                sfRenderWindow_Close(Game);
            }

            // Mettre ici tous les autres events à gérer
        }

        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran

        object_screen_Draw(object_screen);

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre
    }

    // Nettoyage des ressources
    sfRenderWindow_Destroy(Game);
    image_Destroy(image_object);
    sfImage_Destroy(fond_menu);

    return EXIT_SUCCESS;

}
