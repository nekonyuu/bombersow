#include "SFML/Graphics.h"
#include "SFML/Audio.h"
#include "Game/MenuScreen.h"
#include "Objects/Screen.h"
#include "GraphicEngine/Image.h"

int main()
{
    sfWindowSettings Settings = {24, 8, 0};
    sfVideoMode Mode = {900, 600, 32};
    sfRenderWindow* Game;

    Image *image = image_Create();
    map_Loader_Image(image, "test.txt");
    image_Destroy(image);

    // Création de la fenêtre principale
    Game = sfRenderWindow_Create(Mode, "BomberSow", sfClose, Settings);
    if (!Game)
        return EXIT_FAILURE;

    // Démarrage du jeu
    // armory_Create(armory);       // Remplissage du tableau
    display_Menu(Game);

    // Nettoyage des ressources
    sfRenderWindow_Destroy(Game);
    // armory_Destroy(armory);      // Destruction du tableau

    return EXIT_SUCCESS;
}
