#include "SFML/Graphics.h"
#include "SFML/Audio.h"
#include "Screen/Objects_Screen.h"
#include "Screen/Menu_Screen.h"
#include "File/file.h"
#include "Gui/Gui.h"

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

    sfImage *fond_object = sfImage_CreateFromFile("base/fond_object.png");
    Object_Screen* object_screen = object_screen_Create(Game, fond_object, 0, 600, 1100, 200);
    object_screen_Load_Object(object_screen, image_object);

    Image *image_menu = image_Create();
    char *image_text[] = {"base/fond_menu.png", "base/textbox_back.png", "base/fond_menu_bouton.png", "base/slide_top.png", "base/slide_middle.png", "base/slide_bottom.png"};
    image_Loader(image_menu, image_text, 6);

    Editor* editor = editor_Create();
    Object_Menu* menu_screen = menu_screen_Create(Game, image_menu, 0, 0, 200, 600, editor);

    //TEST GUI
    Widget_slide* slide = widget_slide_Create(0, 0, 11, 100, 3, sfColor_FromRGB(85, 137, 199), image_Get(image_menu, 3), image_Get(image_menu, 5), image_Get(image_menu, 4));
    //FIN TEST GUI


    if (!Game)
        return EXIT_FAILURE;

    sfRenderWindow_SetFramerateLimit(Game, 60);
    // Démarrage du jeu

    int id_image = 0;
    sfSprite* temp_sprite = sfSprite_Create();
    sfSprite_SetImage(temp_sprite, image_Get(image_object, id_image));

    editor->selected_id = 0;
    editor->selected_image = temp_sprite;
    editor->selected_type = 1;

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

                int id_temp = object_screen_Click(object_screen, Event.MouseButton.X, Event.MouseButton.Y) ;
                if (id_temp != -1)
                {
                    id_image = id_temp;
                    temp_sprite = sfSprite_Create();
                    sfSprite_SetImage(temp_sprite, image_Get(image_object, id_image));
                    editor->selected_image = temp_sprite;
                    editor->selected_id = id_image;
                    editor->selected_type = 1;
                }
                menu_screen_Click(menu_screen, Event.MouseButton.X, Event.MouseButton.Y) ;
                widget_slide_Click(slide, Event.MouseButton.X, Event.MouseButton.Y) ;
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

        editor_Draw(Game, editor);

        widget_slide_Draw(Game, slide);

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre
    }

    // Nettoyage des ressources
    sfRenderWindow_Destroy(Game);

    object_screen_Destroy(object_screen);
    menu_screen_Destroy(menu_screen);

    image_Destroy(image_object);

    sfImage_Destroy(fond_object);

    sfSprite_Destroy(temp_sprite);

    editor_Destroy(editor);

    widget_slide_Destroy(slide);


    return EXIT_SUCCESS;

}
