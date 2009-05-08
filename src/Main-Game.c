#include "BaseSystem/Logging.h"
#include "Game/GameScreens.h"
#include "Objects/Screen.h"
#include "Map/MapLoader.h"
#include "GraphicEngine/Draw.h"
#include "Objects/GameObjects.h"

int main()
{
    sfWindowSettings Settings = {24, 8, 0};
    sfVideoMode Mode = {SCREEN_WIDTH, SCREEN_HEIGHT, 32};
    sfRenderWindow* Game;

    Image *image = image_Create();
    map_Loader_Image(image, "test.txt");
    image_Destroy(image);

    // Création de la fenêtre principale
    Game = sfRenderWindow_Create(Mode, GAME_NAME, sfClose, Settings);
    //sfRenderWindow_SetFramerateLimit(Game, FRAMERATE);
    if (!Game)
        return EXIT_FAILURE;

    // Démarrage du jeu
    armory_Create(armory);          // Remplissage de l'armurerie
    logging_Info("main", "Game Start");
    display_Menu(Game);

    // Nettoyage des ressources
    sfRenderWindow_Close(Game);     // Fermeture de la fenêtre
    sfRenderWindow_Destroy(Game);
    armory_Destroy(armory);         // Destruction de l'armurerie

#ifdef DEBUG_MODE
    ConteneurMalloc_Affiche (true);
#endif

    return EXIT_SUCCESS;
}
