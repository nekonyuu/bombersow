#include "SFML/Graphics.h"
#include "SFML/Audio.h"
#include "Screen/Objects_Screen.h"
#include "File/file.h"
#include "Gui/Gui.h"


void test(int *a){
        printf("TEST%dTEST\n", *a);
}

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




    //TEST GUI
    sfImage *image_cadre = sfImage_CreateFromFile("base/gui.png");
    sfSprite *background = sfSprite_Create();
    sfSprite_SetImage(background, image_cadre);
    sfIntRect rect = {0, 0, 127, 127};
    sfSprite_SetSubRect(background, &rect);

    int var_test = 10;
    char var_test_char[100] = "Salut";
    Widget_textbox *textbox = widget_textbox_Create(10, 60, 100, 40, 100, image_cadre,background, INT, &var_test);
    Widget_textbox *textbox2 = widget_textbox_Create(10, 10, 100, 40, 100, image_cadre,background, CHAR, &var_test_char);

    void (*p)(void*);
    p = (void*) &test;

    sfIntRect rect2 = {100, 100, 227, 227};
    Widget_bouton *bouton = widget_bouton_Create(rect2, p, &var_test, fond_menu, fond_menu, image_cadre);

    Gui *gui = gui_Create();
    gui_Add_Textbox(gui, textbox);
    gui_Add_Textbox(gui, textbox2);
    gui_Add_Bouton(gui, bouton);
    //FIN TEST GUI


    if (!Game)
        return EXIT_FAILURE;

    sfRenderWindow_SetFramerateLimit(Game, 60);
    // Démarrage du jeu

    int id_image = 0;
    sfSprite* temp_sprite = sfSprite_Create();
    sfSprite_SetImage(temp_sprite, image_Get(image_object, id_image));

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
                gui_TextEntered(gui, Event.Text.Unicode);
            }

            if (Event.Type == sfEvtMouseButtonPressed)
            {

                int id_temp = object_screen_Click(object_screen, Event.MouseButton.X, Event.MouseButton.Y) ;
                if (id_temp != -1)
                {
                    id_image = id_temp;
                    sfSprite_SetImage(temp_sprite, image_Get(image_object, id_image));
                }
                gui_Click(gui, Event.MouseButton.X, Event.MouseButton.Y);

            }

            if (Event.Type == sfEvtMouseMoved)
            {
                sfSprite_SetPosition(temp_sprite, Event.MouseMove.X, Event.MouseMove.Y);
            }

            // Mettre ici tous les autres events à gérer
        }

        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'écran

        object_screen_Draw(object_screen);


        sfRenderWindow_DrawSprite(Game, temp_sprite);

        gui_Draw(Game, gui);

        sfRenderWindow_Display(Game);                           // Mise à jour de la fenêtre
    }

    // Nettoyage des ressources
    sfRenderWindow_Destroy(Game);

    object_screen_Destroy(object_screen);

    image_Destroy(image_object);

    sfImage_Destroy(fond_menu);
    sfImage_Destroy(image_cadre);

    sfSprite_Destroy(temp_sprite);
    sfSprite_Destroy(background);

    gui_Destroy(gui);

    return EXIT_SUCCESS;

}
