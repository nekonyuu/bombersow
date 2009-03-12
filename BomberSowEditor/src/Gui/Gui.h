#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <stdlib.h>
#include "SFML/Graphics.h"

// Type de Widget
typedef enum WIDGET_TYPE {BOUTON, TEXTBOX} Widget_Type;

// Type de textbox
typedef enum WIDGET_TEXTBOX_TYPE {INT, CHAR, STRING}Widget_textbox_type;

// Variable associé à une textbox
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

    sfSprite *cadre;

    Widget_textbox_var *var;        // Variable associé
    char *text_char;
    int taille;
    sfString *text;                 // Texte de la textbox

    _Bool active;                   // Booléen de seleciton

}Widget_textbox;


Widget_textbox_var* widget_text_box_var_Create(Widget_textbox_type, void*);
void widget_textbox_var_Destroy(Widget_textbox_var*);
void widget_textbox_var_Get(Widget_textbox_var*, Widget_textbox*);
void widget_textbox_var_Set(Widget_textbox_var*, Widget_textbox*);

/*
typedef struct WIDGET_BOUTON {

    int x;
    int y;

    void* onClick_Callback;
};*/

Widget_textbox* widget_textbox_Create(int, int, int, int, int, sfImage*, Widget_textbox_type, void*);
void widget_textbox_Destroy(Widget_textbox*);
void widget_textbox_Click(Widget_textbox*, int, int);
void widget_textbox_Write(Widget_textbox*, sfUint32);
_Bool widget_textbox_Check(Widget_textbox*);
void widget_textbox_Draw(sfRenderWindow*, Widget_textbox*);

//Widget
typedef struct WIDGET
{

    Widget_Type widget_type;        // Type de widget
    //Widget_bouton *bouton;          // Widget bouton
    Widget_textbox *textbox;

}Widget;

//Gui
typedef struct GUI
{

    Widget** widget;                // Tableau de tout les widgets du Gui
    int widget_nombre;              // Nombre de widgets

} Gui;

#endif
