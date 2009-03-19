#include <Gui/Gui.h>
#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include "Screen/Objects_Screen.h"
#include "Screen/Menu_Screen.h"
#include "Screen/Map_Screen.h"
#include "Map/MapLoader.h"

int main()
{
    sfWindowSettings Settings = {24, 8, 0};
    sfVideoMode Mode = {1100, 800, 32};
    sfRenderWindow* Game;

    sfEvent Event;

    //Chargement des images des objets
    Image *image_object = image_Create();
    dossier_Read_Image(image_object, "base/images/");

    //Chargement des images des menu
    Image *image_menu = image_Create();
    char *image_text[] = {"base/fond_menu.png", "base/textbox_back.png", "base/fond_menu_bouton.png", "base/slide_top.png", "base/slide_middle.png", "base/slide_bottom.png", "base/fond_object.png", "base/blank_map.png", "base/fond_menu_bouton2.png"};
    image_Loader(image_menu, image_text, 9);


    Editor* editor = editor_Create();   //Création de l'objet editor

    // Création de la fenêtre principale
    Game = sfRenderWindow_Create(Mode, "BomberSowEditor", sfClose, Settings);

    //Création du screen object
    Object_Screen* object_screen = object_screen_Create(Game, image_Get(image_menu, 6), 0, 600, 1100, 200, image_menu, editor);
    object_screen_Load_Object(object_screen, image_object); //Load des image


    Object_Menu* menu_screen = menu_screen_Create(Game, image_menu, 0, 0, 200, 600, editor, object_screen); // Création du screen menu

    Map_screen* map_screen = map_screen_Create(Game, image_Get(image_menu, 7), editor, 200, 0, 900, 600);   // Création du screen map


    if (!Game)
        return EXIT_FAILURE;

    //Set des option de la fenetre
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

            if (Event.Type == sfEvtTextEntered)
            {
                gui_TextEntered(menu_screen->gui, Event.Text.Unicode);
            }

            if (Event.Type == sfEvtMouseButtonPressed)
            {

                object_screen_Click(object_screen, Event.MouseButton.X, Event.MouseButton.Y);
                menu_screen_Click(menu_screen, Event.MouseButton.X, Event.MouseButton.Y) ;
                map_screen_Click(map_screen, Event.MouseButton.X, Event.MouseButton.Y) ;
            }

            if (Event.Type == sfEvtMouseMoved)
            {
                editor_MouseMove(editor, Event.MouseMove.X, Event.MouseMove.Y);
                menu_screen_MouseOver(menu_screen, Event.MouseMove.X, Event.MouseMove.Y);
            }

            // Mettre ici tous les autres events à gérer
        }

        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran

        object_screen_Draw(object_screen);
        menu_screen_Draw(menu_screen);

        for(int i = 0; i < 3; i++){
            map_screen_DrawPlan(map_screen, i);
            if(i == editor->current_plan)
                editor_Draw(Game, editor);
        }

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre
    }

    // Nettoyage des ressources
    sfRenderWindow_Destroy(Game);

    //Destroy des screen
    object_screen_Destroy(object_screen);
    menu_screen_Destroy(menu_screen);
    map_screen_Destroy(map_screen);

    //Destroy du gestionnaire d'image des objet
    image_Destroy(image_object);

    //Destroy de l'objet editor
    editor_Destroy(editor);

    return EXIT_SUCCESS;

}
