#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SFML/Graphics.h"

// Type de Widget
typedef enum WIDGET_TYPE {BOUTON, TEXTBOX} Widget_Type;

// Type de textbox
typedef enum WIDGET_TEXTBOX_TYPE {INT, CHAR, STRING} Widget_textbox_type;


// Cadre et background des Widget
typedef enum CADRE {TOP, LEFT, RIGHT, BOTTOM, TL, TR, BL, BR} cadre;

typedef struct WIDGET_CADRE
{
    sfSprite *sprite_cadre[8];
    sfSprite *background;
    int x;
    int y;
    int width;
    int height;

} Widget_cadre;

Widget_cadre* widget_cadre_Create(sfImage* image, sfSprite*, int, int, int, int);
void widget_cadre_Destroy(Widget_cadre*);
void widget_cadre_Draw(sfRenderWindow*, Widget_cadre*);

// Variable associ� � une textbox
typedef struct WIDGET_TEXTBOX_VAR
{
    int *var_int;
    sfString *var_string;
    char *var_char;
    Widget_textbox_type type;

} Widget_textbox_var;


// Widget Textbox
typedef struct WIDGET_TEXTBOX
{
    int x;
    int y;
    int width;
    int height;

    Widget_cadre *cadre;

    Widget_textbox_var *var;        // Variable associ�
    char *text_char;                // Utile ? Pr�sent dans Widget_textbox_var
    int taille;
    sfString *text;                 // Texte de la textbox (Utile ? Pr�sent dans Widget_textbox_var)

    _Bool active;                   // Bool�en de seleciton

}Widget_textbox;


Widget_textbox_var* widget_text_box_var_Create(Widget_textbox_type, void*);
void widget_textbox_var_Destroy(Widget_textbox_var*);
void widget_textbox_var_Get(Widget_textbox_var*, Widget_textbox*);
void widget_textbox_var_Set(Widget_textbox_var*, Widget_textbox*);

Widget_textbox* widget_textbox_Create(int, int, int, int, int, sfImage*, sfSprite*, Widget_textbox_type, void*);
void widget_textbox_Destroy(Widget_textbox*);
void widget_textbox_Click(Widget_textbox*, int, int);
void widget_textbox_Write(Widget_textbox*, sfUint32);
_Bool widget_textbox_Check(Widget_textbox*);
void widget_textbox_Draw(sfRenderWindow*, Widget_textbox*);


//Widget bouton
typedef enum BOUTON_ACTION {NOTHING, OVER, CLICK}Bouton_action;
typedef struct WIDGET_BOUTON {

    sfIntRect rect;

    sfSprite *sprite_OnOver;
    sfSprite *sprite_OnClick;
    sfSprite *sprite_OnNothing;

    Bouton_action On;

    void (*onClick_Callback)(void*);
    void *onClick_Callback_arg;
}Widget_bouton;

Widget_bouton* widget_bouton_Create(sfIntRect, void (*)(void*), void*, sfImage*, sfImage*, sfImage*);
void widget_bouton_Destroy(Widget_bouton*);


//Gui
typedef struct GUI
{
    Widget_textbox** widget_textbox;                // Tableau de tout les widgets du Gui
    int widget_textbox_nombre;                      // Nombre de widgets

    Widget_bouton** widget_bouton;
    int widget_bouton_nombre;

} Gui;

Gui* gui_Create();
void gui_Destroy(Gui*);
void gui_Load_Textbox(Gui*, Widget_textbox**, int);
void gui_Add_Textbox(Gui*, Widget_textbox*);
void gui_Load_Bouton(Gui*, Widget_bouton**, int);
void gui_Add_Bouton(Gui*, Widget_bouton*);
void gui_Draw(sfRenderWindow*, Gui*);
void gui_Click(Gui*, int, int);
void gui_TextEntered(Gui*, sfUint32);
#endif
