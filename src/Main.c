#include "SFML/Graphics.h"
#include "SFML/Audio.h"

#include "Game/MenuScreen.h"
#include "Objects/Screen.h"
#include "Objects/Weapon.h"
#include "GraphicEngine/Image.h"
#include "Objects/Map.h"

int main()
{
    sfWindowSettings Settings = {24, 8, 0};
    sfVideoMode Mode = {900, 600, 32};
    sfRenderWindow* Game;

    Image *image = image_Create();
    map_Loader_Image(image, "test.txt");
    image_Destroy(image);

    // Cr�ation de la fen�tre principale
    Game = sfRenderWindow_Create(Mode, "BomberSow", sfClose, Settings);
    if (!Game)
        return EXIT_FAILURE;

    // D�marrage du jeu
    armory_Create(armory);          // Remplissage de l'armurerie
    display_Menu(Game);

    // Nettoyage des ressources
    sfRenderWindow_Destroy(Game);
    armory_Destroy(armory);         // Destruction de l'armurerie

    return EXIT_SUCCESS;
}
