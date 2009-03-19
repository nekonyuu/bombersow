#include <assert.h>
#include "SFML/Graphics.h"
#include "Gui/Gui.h"
#include "GraphicEngine/Image.h"
#include "Screen/Menu_Screen.h"
#include "Screen/Objects_Screen.h"
#include "Objects/Editor.h"

//$order->products[$i]['qty']
/*
[IMAGES]
ID PATH
0=image par défault

[ANIMATION]
ID      ID_IMAGE    X       Y       HAUTEUR     LARGEUR     NOMBRE_IMAGE    FPS
int     int         int     int     int         int         int             float

[SOUNDS]
ID PATH
0=sounds par défault

[BACKGROUND]
ID_IMAGE
int

[OBJECTS]
TYPE    ID_ANIMATION    ID_IMAGE        LARGEUR     HAUTEUR     [PARAMETRE OPTIONNEL]
int     int             int             int         int         [PARA]
//Si id_animation = -1, image fixe id_image != -1
//si id_animation !=-1, id_image = -1

[ELEMENTS]
ID_OBJECT      X       Y      PLAN     [PARAMETRE OPTIONNEL]
int            int    int     int      [para en plus suivant le type]

*/

Object_Menu* menu_screen_Create(sfRenderWindow* Game, Image* image, int x, int y, int largeur, int hauteur, Editor *editor, Object_Screen* object_screen)
{

    Object_Menu* menu = NULL;
    assert(menu = (Object_Menu*) malloc(sizeof(Object_Menu)));

    menu->Game = Game;

    menu->editor = editor;

    menu->x = x;
    menu->y = y;

    menu->largeur = largeur;
    menu->hauteur = hauteur;

    menu->image = image;

    menu->background = sfSprite_Create();
    sfImage_SetSmooth(image_Get(menu->image, MENU_BACKGROUND), false);
    sfSprite_SetImage(menu->background, image_Get(menu->image, MENU_BACKGROUND));
    sfSprite_SetPosition(menu->background, x, y);

    menu->object_screen = object_screen;;

    menu->gui = gui_Create();

    //Widget animation editor
    animation_Destroy(editor->animation_create);
    editor->animation_create = animation_Create(image_Get(image, 0), 0, 0, 0, 0, 0 ,0 ,0 ,0);

    Widget_textbox *textbox = widget_textbox_Create(10, 65, 25, 15, 100, image_Get(menu->image, MENU_TEXTBOX_BACKGROUND), sfColor_FromRGB(0,0,0), INT, &editor->animation_create->image_largeur, "Largeur :", sfBlack, NULL, 12);
    gui_Add_Textbox(menu->gui, textbox);

    Widget_textbox *textbox2 = widget_textbox_Create(90, 65, 25, 15, 100, image_Get(menu->image, MENU_TEXTBOX_BACKGROUND), sfColor_FromRGB(0,0,0), INT, &editor->animation_create->image_hauteur, "Hauteur :", sfBlack, NULL, 12);
    gui_Add_Textbox(menu->gui, textbox2);

    Widget_textbox *textbox3 = widget_textbox_Create(110, 85, 25, 15, 100, image_Get(menu->image, MENU_TEXTBOX_BACKGROUND), sfColor_FromRGB(0,0,0), INT, &editor->animation_create->nombre_image, "Nbr img :", sfBlack, NULL, 12);
    gui_Add_Textbox(menu->gui, textbox3);

    Widget_textbox *textbox4 = widget_textbox_Create(10, 85, 25, 15, 100, image_Get(menu->image, MENU_TEXTBOX_BACKGROUND), sfColor_FromRGB(0,0,0), INT, &editor->animation_create->x, "X :", sfBlack, NULL, 12);
    gui_Add_Textbox(menu->gui, textbox4);

    Widget_textbox *textbox5 = widget_textbox_Create(60, 85, 25, 15, 100, image_Get(menu->image, MENU_TEXTBOX_BACKGROUND), sfColor_FromRGB(0,0,0), INT, &editor->animation_create->y, "Y :", sfBlack, NULL, 12);
    gui_Add_Textbox(menu->gui, textbox5);

    Widget_textbox *textbox6 = widget_textbox_Create(10, 105, 80, 15, 100, image_Get(menu->image, MENU_TEXTBOX_BACKGROUND), sfColor_FromRGB(0,0,0), FLOAT, &editor->animation_create->fps, "FPS :", sfBlack, NULL, 12);
    gui_Add_Textbox(menu->gui, textbox6);


    //Widget objects editor
    void *bouton_anim = (void*) &object_screen_Switch;
    sfIntRect rect_bouton = {10, 155, 60, 170};
    Widget_bouton *bouton = widget_bouton_Create(rect_bouton, bouton_anim, menu->object_screen, NULL, image_Get(menu->image, 8), image_Get(menu->image, 8), image_Get(menu->image, 8));
    gui_Add_Bouton(menu->gui, bouton);

    textbox = widget_textbox_Create(10, 175, 25, 15, 100, image_Get(menu->image, MENU_TEXTBOX_BACKGROUND), sfColor_FromRGB(0,0,0), INT, &editor->current_plan, "Plan :", sfBlack, NULL, 12);
    gui_Add_Textbox(menu->gui, textbox);

    textbox = widget_textbox_Create(70, 175, 25, 15, 100, image_Get(menu->image, MENU_TEXTBOX_BACKGROUND), sfColor_FromRGB(0,0,0), INT, &editor->object_create->type, "Type :", sfBlack, NULL, 12);
    gui_Add_Textbox(menu->gui, textbox);

    textbox = widget_textbox_Create(10, 195, 25, 15, 100, image_Get(menu->image, MENU_TEXTBOX_BACKGROUND), sfColor_FromRGB(0,0,0), INT, &editor->bool_dynamic, "Dynamique :", sfBlack, NULL, 12);
    gui_Add_Textbox(menu->gui, textbox);

    textbox = widget_textbox_Create(110, 195, 25, 15, 100, image_Get(menu->image, MENU_TEXTBOX_BACKGROUND), sfColor_FromRGB(0,0,0), INT, &editor->object_create->speed, "Speed :", sfBlack, NULL, 12);
    gui_Add_Textbox(menu->gui, textbox);

    textbox = widget_textbox_Create(10, 215, 25, 15, 100, image_Get(menu->image, MENU_TEXTBOX_BACKGROUND), sfColor_FromRGB(0,0,0), INT, &editor->object_create->weapon_id, "Weapon :", sfBlack, NULL, 12);
    gui_Add_Textbox(menu->gui, textbox);

     textbox = widget_textbox_Create(100, 215, 25, 15, 100, image_Get(menu->image, MENU_TEXTBOX_BACKGROUND), sfColor_FromRGB(0,0,0), INT, &editor->object_create->nb_ammo, "Ammo :", sfBlack, NULL, 12);
    gui_Add_Textbox(menu->gui, textbox);


    void *p = (void*) &object_screen_Add_Animation;
    sfIntRect rect2 = {130, 105, 155, 120};
    bouton = widget_bouton_Create(rect2, p, menu->object_screen, editor->animation_create, image_Get(menu->image, MENU_BOUTON), image_Get(menu->image, MENU_BOUTON), image_Get(menu->image, MENU_BOUTON));

    gui_Add_Bouton(menu->gui, bouton);


    return menu;

}

void menu_screen_Destroy(Object_Menu* menu)
{

    sfSprite_Destroy(menu->background);
    gui_Destroy(menu->gui);
    image_Destroy(menu->image);

    free(menu);
}

void menu_screen_Draw(Object_Menu* menu)
{

    sfRenderWindow_DrawSprite(menu->Game, menu->background);

    gui_Draw(menu->Game, menu->gui);

}

void menu_screen_Click(Object_Menu* menu, int mouse_x, int mouse_y)
{

    sfIntRect cadre_screen = {menu->x, menu->y, menu->x+menu->largeur, menu->y+menu->hauteur};
    if (sfIntRect_Contains(&cadre_screen, mouse_x, mouse_y))
        gui_Click(menu->gui, mouse_x, mouse_y);
}

void menu_screen_MouseOver(Object_Menu* menu, int mouse_x, int mouse_y)
{

    sfIntRect cadre_screen = {menu->x, menu->y, menu->x+menu->largeur, menu->y+menu->hauteur};
    if (sfIntRect_Contains(&cadre_screen, mouse_x, mouse_y))
    {

        gui_MouseOver(menu->gui, mouse_x, mouse_y);

    }

}
