/*
    GPL v3 Licence :
    Bombersow is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bombersow is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bombersow.  If not, see <http://www.gnu.org/licenses/>.


    Creative Commons BY-NC-SA :
    This work is licensed under the Creative Commons Attribution-Noncommercial-Share Alike 3.0 Unported License.
    To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter
    to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.

*/

#include <time.h>
#include <SFML/Graphics.h>

#include "BaseSystem/Config.h"
#include "BaseSystem/Logging.h"
#include "GraphicEngine/Draw.h"
#include "Game/GameScreens.h"
#include "Objects/Screen.h"
#include "Map/MapLoader.h"
#include "GraphicEngine/Draw.h"
#include "Objects/GameObjects.h"

int main()
{
    logging_Info("main", "Load config");
    // TODO : Loader de config, Ludo on t'attends =D
    /* -------- START CONFIG -------- */
    Config* game_config;
    assert(game_config = (Config*) malloc(sizeof(Config)));
    game_config->width = 900;
    game_config->height = 675;
    game_config->vsync = sfFalse;
    game_config->fps_limit = 125;
    game_config->show_fps = true;
    game_config->move_speed = 425;
    game_config->jump_speed = -85;
    game_config->force_fall_speed = 250;
    game_config->gravity_speed = 275.f;
    game_config->gravity_coef = 6.66f;
    /* --------- END CONFIG --------- */

    logging_Info("main", "Initializing rand...");
    srand(time(NULL));

    /* -------- Player Image -------- */
    logging_Info("main", "Load default sprite");
    player_default_image = sfImage_CreateFromFile("base/images/animation2.png");

    logging_Info("main", "Set Display parameters");
    sfWindowSettings Settings = {24, 8, 2};
    sfVideoMode Mode = {game_config->width, game_config->height, 32};
    sfRenderWindow* Game;

    /*Image *image = image_Create();
    map_Loader_Image(image, "test.txt");
    image_Destroy(image);*/

    // Création de la fenêtre principale
    logging_Info("main", "Create Display window");
    Game = sfRenderWindow_Create(Mode, GAME_NAME, sfClose, Settings);

    logging_Info("main", "Set framerate parameters");
    sfRenderWindow_UseVerticalSync(Game, game_config->vsync);
    if (!game_config->vsync)
        sfRenderWindow_SetFramerateLimit(Game, game_config->fps_limit);

    logging_Info("main", "Verify Window state");
    if (!Game)
        return DISPLAY_FAIL_ERROR;

    GraphicEngine_Init(Game, game_config);

    // Démarrage du jeu
    armory_Create(armory);          // Remplissage de l'armurerie
    armory_LoadImages(weap_img);    // Remplissage du tableau d'images de base
    logging_Info("main", "Game Start");
    game_MainMenu(Game, game_config);

    // Nettoyage des ressources
    sfRenderWindow_Close(Game);     // Fermeture de la fenêtre
    sfRenderWindow_Destroy(Game);
    sfImage_Destroy(player_default_image);
    armory_Destroy(armory);
    armory_DestroyImages(weap_img);
    free_secure(game_config);

#ifdef HALLOC_DBG
    ConteneurMalloc_Affiche (true);
#endif

    return NO_ERROR;
}
