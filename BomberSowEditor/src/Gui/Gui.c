#include <assert.h>
#include <string.h>
#include "SFML/Graphics.h"
#include "BaseSystem/Logging.h"
#include "Gui/Gui.h"

#define UNI32_RETURN 8

Widget_cadre* widget_cadre_Create(sfImage* image, sfSprite* sprite_background, int x, int y, int width, int height)
{

    Widget_cadre* cadre = NULL;
    assert(cadre = (Widget_cadre*) malloc(sizeof(Widget_cadre)));

    for (int i = 0; i < 8; i++)
    {
        cadre->sprite_cadre[i] = sfSprite_Create();
        sfSprite_SetImage(cadre->sprite_cadre[i], image);
    }

    cadre->background = sfSprite_Create();
    sfSprite_SetImage(cadre->background, sfSprite_GetImage(sprite_background));
    sfSprite_SetSubRect(cadre->background, sfSprite_GetSubRect(sprite_background));

    cadre->x = x;
    cadre->y = y;
    cadre->width = width;
    cadre->height = height;

    sfIntRect rect = {131, 0, 188, 3};
    sfSprite_SetSubRect(cadre->sprite_cadre[TOP], &rect);

    sfIntRect rect2 = {131, 61, 188, 64};
    sfSprite_SetSubRect(cadre->sprite_cadre[BOTTOM], &rect2);

    sfIntRect rect3 = {128, 3, 131, 60};
    sfSprite_SetSubRect(cadre->sprite_cadre[LEFT], &rect3);

    sfIntRect rect4 = {188, 3, 192, 60};
    sfSprite_SetSubRect(cadre->sprite_cadre[RIGHT], &rect4);

    sfIntRect rect5 = {128, 0, 130, 2};
    sfSprite_SetSubRect(cadre->sprite_cadre[TL], &rect5);

    sfIntRect rect6 = {128, 61, 130, 63};
    sfSprite_SetSubRect(cadre->sprite_cadre[TR], &rect6);

    sfIntRect rect7 = {189, 0, 192, 3};
    sfSprite_SetSubRect(cadre->sprite_cadre[BL], &rect7);

    sfIntRect rect8 = {189, 61, 192, 63};
    sfSprite_SetSubRect(cadre->sprite_cadre[BR], &rect8);

    sfSprite_SetPosition(cadre->sprite_cadre[TL], x, y);
    sfSprite_SetPosition(cadre->sprite_cadre[TR], x+width, y);
    sfSprite_SetPosition(cadre->sprite_cadre[BL], x, y+height);
    sfSprite_SetPosition(cadre->sprite_cadre[BR], x+width, y+height);

    sfSprite_SetPosition(cadre->sprite_cadre[TOP], x+1, y);
    sfSprite_SetScaleX(cadre->sprite_cadre[TOP], width/sfSprite_GetWidth(cadre->sprite_cadre[TOP]));

    sfSprite_SetPosition(cadre->sprite_cadre[BOTTOM], x+1, y+height);
    sfSprite_SetScaleX(cadre->sprite_cadre[BOTTOM], width/sfSprite_GetWidth(cadre->sprite_cadre[BOTTOM]));

    sfSprite_SetPosition(cadre->sprite_cadre[LEFT], x, y+1);
    sfSprite_SetScaleY(cadre->sprite_cadre[LEFT], height/sfSprite_GetHeight(cadre->sprite_cadre[LEFT]));

    sfSprite_SetPosition(cadre->sprite_cadre[RIGHT], x+width, y+1);
    sfSprite_SetScaleY(cadre->sprite_cadre[RIGHT], height/sfSprite_GetHeight(cadre->sprite_cadre[RIGHT]));

    sfSprite_SetPosition(cadre->background, x+1, y+1);
    sfSprite_SetScale(cadre->background, width/sfSprite_GetWidth(cadre->background), height/sfSprite_GetHeight(cadre->background));

    return cadre;
}

void widget_cadre_Destroy(Widget_cadre* cadre)
{
    if (!cadre)
    {
        logging_Warning("widget_cadre_Destroy", "Widget_cadre object sent NULL");
        return;
    }

    for (int i = 0; i < 8; i++)
    {
        sfSprite_Destroy(cadre->sprite_cadre[i]);
    }

    sfSprite_Destroy(cadre->background);

    free(cadre);
}

void widget_cadre_Draw(sfRenderWindow* Game, Widget_cadre* cadre)
{
    assert(Game && cadre);

    sfRenderWindow_DrawSprite(Game, cadre->background);
    for (int i = 0; i < 8; i++)
        sfRenderWindow_DrawSprite(Game, cadre->sprite_cadre[i]);
}

// Construit la Widget_textbox_var
Widget_textbox_var* widget_text_box_var_Create(Widget_textbox_type type, void* var)
{
    Widget_textbox_var *textbox_var = NULL;
    assert(textbox_var = (Widget_textbox_var*) malloc(sizeof(Widget_textbox_var)));

    switch (type)
    {
    case INT:
        textbox_var->type = INT;
        textbox_var->var_int = (int*) var;
        textbox_var->var_string = NULL;
        textbox_var->var_char = NULL;
        break;

    case STRING:
        textbox_var->type = STRING;
        textbox_var->var_int = NULL;
        textbox_var->var_string = (sfString*) var;
        textbox_var->var_char = NULL;
        break;

    case CHAR:
        textbox_var->type = CHAR;
        textbox_var->var_int = NULL;
        textbox_var->var_string = NULL;
        textbox_var->var_char = (char*) var;
        break;
    }

    return textbox_var;
}

// Détruit la Widget_textbox_var
void widget_textbox_var_Destroy(Widget_textbox_var* textbox_var)
{
    if (!textbox_var)
    {
        logging_Warning("widget_textbox_var_Destroy", "Widget_textbox_var object sent NULL");
        return;
    }

    /*if(textbox_var->var_char)
        free(textbox_var->var_char);
    if(textbox_var->var_int)
        free(textbox_var->var_int);
    if(textbox_var->var_string)
        sfString_Destroy(textbox_var->var_string);*/

    free(textbox_var);
    textbox_var = NULL;
}

// Récupère la valeur active de la Widget_textbox_var
void widget_textbox_var_Get(Widget_textbox_var* textbox_var, Widget_textbox* textbox)
{
    if (textbox_var->type == INT)
    {
        sprintf(textbox->text_char, "%d", *textbox_var->var_int);
        sfString_SetText(textbox->text, textbox->text_char);
    }
    else if (textbox_var->type == CHAR)
    {
        sfString_SetText(textbox->text, textbox_var->var_char);
        strcpy(textbox->text_char, textbox_var->var_char);
    }
    else
    {
        textbox->text = textbox_var->var_string;
    }
}

void widget_textbox_var_Set(Widget_textbox_var* textbox_var, Widget_textbox* textbox)
{
    switch (textbox_var->type)
    {
    case INT:
        *textbox_var->var_int = atoi(textbox->text_char);
        break;

    case STRING:
        // textbox_var->var_string détruit dans widget_textbox_var_Destroy, utilisé par widget_textbox_Destroy
        textbox_var->var_string = textbox->text;
        break;

    case CHAR:
        strcpy(textbox_var->var_char, textbox->text_char);
        break;
    }
}


Widget_textbox* widget_textbox_Create(int x, int y, int width, int height, int taille, sfImage* image, sfSprite* sprite_background, Widget_textbox_type type, void* var)
{
    Widget_textbox* textbox = NULL;
    assert(textbox = malloc(sizeof(Widget_textbox)));

    textbox->text = sfString_Create();
    sfString_SetPosition(textbox->text, x+5, y);
    sfString_SetColor(textbox->text, sfBlack);
    sfString_SetSize(textbox->text, height-5);

    textbox->text_char = NULL;
    assert(textbox->text_char = (char*) malloc(taille*sizeof(char)));

    textbox->taille = taille;

    textbox->x = x;
    textbox->y = y;
    textbox->width = width;
    textbox->height = height;

    textbox->cadre = widget_cadre_Create(image, sprite_background, x, y, width, height);

    textbox->var = widget_text_box_var_Create(type, var);
    widget_textbox_var_Get(textbox->var, textbox);

    textbox->active = false;

    return textbox;
}

void widget_textbox_Destroy(Widget_textbox* textbox)
{
    widget_textbox_var_Destroy(textbox->var);

    sfString_Destroy(textbox->text);

    widget_cadre_Destroy(textbox->cadre);
    textbox->cadre = NULL;

    free(textbox->text_char);
    textbox->text_char = NULL;

    free(textbox);
    textbox = NULL;
}

void widget_textbox_Click(Widget_textbox* textbox, int x, int y)
{
    sfIntRect cadre_screen = {textbox->x, textbox->y, textbox->x+textbox->width, textbox->y+textbox->height};

    if (sfIntRect_Contains(&cadre_screen, x, y))
        textbox->active = true;
    else
        textbox->active = false;
}

void widget_textbox_Write(Widget_textbox* textbox, sfUint32 lettre)
{
    if (textbox->var->type != STRING)
    {
        if (lettre == UNI32_RETURN)
        {
            int taille = strlen(textbox->text_char);
            taille = (taille > 0) ? taille-1 : 0;
            textbox->text_char[taille] = '\0';
            sfString_SetText(textbox->text, textbox->text_char);
            widget_textbox_var_Set(textbox->var, textbox);
        }
        else if ((lettre > 31 && lettre < 128))
        {
            if (strlen(textbox->text_char)+1 < textbox->taille)
            {
                int taille = strlen( textbox->text_char);
                if (textbox->var->type == INT && lettre > 47 && lettre < 58)
                {
                    textbox->text_char[taille] = lettre;
                    textbox->text_char[taille+1] = '\0';
                }
                else if (textbox->var->type == CHAR)
                {
                    textbox->text_char[taille] = lettre;
                    textbox->text_char[taille+1] = '\0';
                }
                sfString_SetText(textbox->text, textbox->text_char);
                widget_textbox_var_Set(textbox->var, textbox);
            }
        }
    }
}

_Bool widget_textbox_Check(Widget_textbox* textbox)
{
    return textbox->active;
}

void widget_textbox_Draw(sfRenderWindow* Game, Widget_textbox* textbox)
{
    assert(Game && textbox);

    widget_cadre_Draw(Game, textbox->cadre);
    sfRenderWindow_DrawString(Game, textbox->text);
}


//Widget bouton

Widget_bouton* widget_bouton_Create(sfIntRect rect, void (*f)(void*), void* arg, sfImage* image_OnOver, sfImage* image_OnClick, sfImage* image_OnNothing)
{
    Widget_bouton *bouton = NULL;
    assert(bouton = (Widget_bouton*) malloc(sizeof(Widget_bouton)));

    bouton->rect = rect;
    bouton->onClick_Callback = f;
    bouton->onClick_Callback_arg = arg;

    bouton->sprite_OnClick = sfSprite_Create();
    sfSprite_SetImage(bouton->sprite_OnClick, image_OnClick);
    sfSprite_Resize(bouton->sprite_OnClick, rect.Right-rect.Left, rect.Bottom-rect.Top);
    sfSprite_SetPosition(bouton->sprite_OnClick, rect.Left, rect.Top);

    bouton->sprite_OnNothing = sfSprite_Create();
    sfSprite_SetImage(bouton->sprite_OnNothing, image_OnNothing);
    sfSprite_Resize(bouton->sprite_OnNothing, rect.Right-rect.Left, rect.Bottom-rect.Top);
    sfSprite_SetPosition(bouton->sprite_OnNothing, rect.Left, rect.Top);

    bouton->sprite_OnOver = sfSprite_Create();
    sfSprite_SetImage(bouton->sprite_OnOver, image_OnOver);
    sfSprite_Resize(bouton->sprite_OnOver, rect.Right-rect.Left, rect.Bottom-rect.Top);
    sfSprite_SetPosition(bouton->sprite_OnOver, rect.Left, rect.Top);

    bouton->On = 0;

    return bouton;
}

void widget_bouton_Destroy(Widget_bouton* bouton)
{
    sfSprite_Destroy(bouton->sprite_OnClick);
    sfSprite_Destroy(bouton->sprite_OnNothing);
    sfSprite_Destroy(bouton->sprite_OnOver);

    free(bouton);
    bouton = NULL;
}

void widget_bouton_Click(Widget_bouton* bouton, int x, int y)
{
    if (sfIntRect_Contains(&bouton->rect, x, y))
        bouton->onClick_Callback(bouton->onClick_Callback_arg);
}

void widget_bouton_Draw(sfRenderWindow* Game, Widget_bouton* bouton)
{

    switch (bouton->On)
    {
    case NOTHING:
        sfRenderWindow_DrawSprite(Game, bouton->sprite_OnNothing);
        break;

    case OVER:
        sfRenderWindow_DrawSprite(Game, bouton->sprite_OnOver);
        break;

    case CLICK:
        sfRenderWindow_DrawSprite(Game, bouton->sprite_OnClick);
        break;
    }
}


//Gui
Gui* gui_Create()
{
    Gui* gui = NULL;
    assert(gui = (Gui*)malloc(sizeof(Gui)));

    gui->widget_textbox = NULL;
    gui->widget_textbox_nombre = 0;

    gui->widget_bouton = NULL;
    gui->widget_bouton_nombre = 0;

    return gui;
}

void gui_Load_Textbox(Gui* gui, Widget_textbox** widget, int taille)
{
    gui->widget_textbox_nombre = taille;
    gui->widget_textbox = widget;
}

void gui_Add_Textbox(Gui* gui, Widget_textbox* widget)
{
    gui->widget_textbox_nombre = gui->widget_textbox_nombre+1;
    assert(gui->widget_textbox = realloc(gui->widget_textbox, gui->widget_textbox_nombre*sizeof(Widget_textbox*)));
    gui->widget_textbox[gui->widget_textbox_nombre-1] = widget;
}

void gui_Load_Bouton(Gui* gui, Widget_bouton** widget, int taille)
{
    gui->widget_bouton_nombre = taille;
    gui->widget_bouton = widget;
}

void gui_Add_Bouton(Gui* gui, Widget_bouton* widget)
{
    if (!gui->widget_bouton)
        assert(gui->widget_bouton = (Widget_bouton**) malloc(++gui->widget_bouton_nombre * sizeof(Widget_bouton*)));
    else
        assert(gui->widget_bouton = (Widget_bouton**) realloc(gui->widget_bouton, ++gui->widget_bouton_nombre * sizeof(Widget_bouton*)));

    gui->widget_bouton[gui->widget_bouton_nombre - 1] = widget;
}

void gui_Destroy(Gui* gui)
{
    for (int i = 0; i < gui->widget_textbox_nombre; i++)
        widget_textbox_Destroy(gui->widget_textbox[i]);

    free(gui->widget_textbox);
    gui->widget_textbox = NULL;

    free(gui);
    gui = NULL;
}

void gui_Draw(sfRenderWindow* Game, Gui* gui)
{
    for (int i = 0; i < gui->widget_textbox_nombre; i++)
        widget_textbox_Draw(Game, gui->widget_textbox[i]);

    for (int i = 0; i < gui->widget_bouton_nombre; i++)
        widget_bouton_Draw(Game, gui->widget_bouton[i]);
}

void gui_Click(Gui* gui, int x, int y)
{
    for (int i = 0; i < gui->widget_textbox_nombre; i++)
        widget_textbox_Click(gui->widget_textbox[i], x, y);

    for (int i = 0; i < gui->widget_bouton_nombre; i++)
        widget_bouton_Click(gui->widget_bouton[i], x, y);
}

void gui_TextEntered(Gui* gui, sfUint32 unicode)
{
    for (int i = 0; i < gui->widget_textbox_nombre; i++)
    {
        if (widget_textbox_Check(gui->widget_textbox[i]))
        {
            widget_textbox_Write(gui->widget_textbox[i], unicode);
        }
    }
}
