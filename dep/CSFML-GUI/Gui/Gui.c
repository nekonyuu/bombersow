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

#include <assert.h>
#include <string.h>
#include "SFML/Graphics.h"
#include "BaseSystem/Logging.h"
#include "Gui/Gui.h"

#define UNI32_RETURN 8

Widget_cadre* widget_cadre_Create(sfImage* image, sfColor couleur, int x, int y, int width, int height)
{

    Widget_cadre* cadre = NULL;
    assert(cadre = (Widget_cadre*) malloc(sizeof(Widget_cadre)));

    cadre->background = sfSprite_Create();
    sfSprite_SetImage(cadre->background, image);
    sfSprite_Resize(cadre->background, width, height);
    sfSprite_SetPosition(cadre->background, x, y);

    cadre->shape = sfShape_CreateRectangle(x, y, x+width, y+height, sfColor_FromRGB(0,0,0), 1, couleur);
    sfShape_EnableFill(cadre->shape, false);

    cadre->x = x;
    cadre->y = y;
    cadre->width = width;
    cadre->height = height;

    return cadre;
}

void widget_cadre_Destroy(Widget_cadre* cadre)
{
    if (!cadre)
    {
        logging_Warning("widget_cadre_Destroy", "Widget_cadre object sent NULL");
        return;
    }

    sfShape_Destroy(cadre->shape);
    sfSprite_Destroy(cadre->background);

    free_secure(cadre);
}

void widget_cadre_Draw(sfRenderWindow* Game, Widget_cadre* cadre)
{
    assert(Game && cadre);

    sfRenderWindow_DrawSprite(Game, cadre->background);
    sfRenderWindow_DrawShape(Game, cadre->shape);
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
        textbox_var->var_float = NULL;
        break;

    case FLOAT:
        textbox_var->type = FLOAT;
        textbox_var->var_int = NULL;
        textbox_var->var_float = (float*) var;
        textbox_var->var_string = NULL;
        textbox_var->var_char = NULL;
        break;

    case STRING:
        textbox_var->type = STRING;
        textbox_var->var_int = NULL;
        textbox_var->var_string = (sfString*) var;
        textbox_var->var_char = NULL;
        textbox_var->var_float = NULL;
        break;

    case CHAR:
        textbox_var->type = CHAR;
        textbox_var->var_int = NULL;
        textbox_var->var_string = NULL;
        textbox_var->var_char = (char*) var;
        textbox_var->var_float = NULL;
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
        free_secure(textbox_var->var_char);
    if(textbox_var->var_int)
        free_secure(textbox_var->var_int);
    if(textbox_var->var_string)
        sfString_Destroy(textbox_var->var_string);*/
    free_secure(textbox_var);
}

// Récupère la valeur active de la Widget_textbox_var
void widget_textbox_var_Get(Widget_textbox_var* textbox_var, Widget_textbox* textbox)
{
    if (textbox_var->type == INT)
    {
        sprintf(textbox->text_char, "%d", *textbox_var->var_int);
        sfString_SetText(textbox->text, textbox->text_char);
    }
    else if (textbox_var->type == FLOAT)
    {
        sprintf(textbox->text_char, "%f", *textbox_var->var_float);
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

    case FLOAT:
        *textbox_var->var_float = atof(textbox->text_char);
        break;

    case STRING:
        textbox_var->var_string = textbox->text;
        break;

    case CHAR:
        strcpy(textbox_var->var_char, textbox->text_char);
        break;
    }
}


Widget_textbox* widget_textbox_Create(int x, int y, int width, int height, int taille, sfImage* bg_image, sfColor couleur_cadre,
                                      Widget_textbox_type type, void* var, sfColor couleur_text, char* texte, sfColor couleur_label, sfFont* font, int texte_size)
{
    Widget_textbox* textbox = NULL;
    assert(textbox = malloc(sizeof(Widget_textbox)));

    textbox->alt = sfString_Create();
    sfString_SetText(textbox->alt, texte);
    sfString_SetColor(textbox->alt, couleur_label);
    sfString_SetSize(textbox->alt, texte_size);
    if (font)
        sfString_SetFont(textbox->alt, font);

    sfFloatRect* rect = sfString_GetRect(textbox->alt);
    sfString_SetPosition(textbox->alt, x, y + ((height - (rect->Bottom - rect->Top)) / 2) - 2);

    rect = sfString_GetRect(textbox->alt);
    rect->Right += 2;

    textbox->text = sfString_Create();
    sfString_SetPosition(textbox->text, rect->Right, y-2);
    sfString_SetColor(textbox->text, couleur_text);
    sfString_SetSize(textbox->text, height-2);
    if (font)
        sfString_SetFont(textbox->text, font);

    textbox->text_char = NULL;
    assert(textbox->text_char = (char*) malloc((taille + 1) * sizeof(char)));

    textbox->taille = taille;

    textbox->x = (int) rect->Right;
    textbox->y = y;
    textbox->width = width;
    textbox->height = height;

    textbox->cadre = widget_cadre_Create(bg_image, couleur_cadre, (int)rect->Right, y, width, height);

    textbox->var = widget_text_box_var_Create(type, var);
    widget_textbox_var_Get(textbox->var, textbox);

    textbox->active = false;

    return textbox;
}

void widget_textbox_Destroy(Widget_textbox* textbox)
{
    widget_textbox_var_Destroy(textbox->var);

    sfString_Destroy(textbox->text);
    sfString_Destroy(textbox->alt);

    widget_cadre_Destroy(textbox->cadre);

    free_secure(textbox->text_char);
    free_secure(textbox);
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
            if (strlen(textbox->text_char) < textbox->taille)
            {
                int taille = strlen(textbox->text_char);
                if (textbox->var->type == INT && lettre > 47 && lettre < 58)
                {
                    textbox->text_char[taille] = lettre;
                    textbox->text_char[taille+1] = '\0';
                }
                else if (textbox->var->type == FLOAT && ((lettre > 47 && lettre < 58) || lettre == '.'))
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

bool widget_textbox_IsActive(Widget_textbox* textbox)
{
    return textbox->active;
}

void widget_textbox_Update(Widget_textbox* textbox)
{
    widget_textbox_var_Get(textbox->var, textbox);
}

void widget_textbox_Draw(sfRenderWindow* Game, Widget_textbox* textbox)
{
    assert(Game && textbox);

    sfRenderWindow_DrawString(Game, textbox->alt);
    widget_cadre_Draw(Game, textbox->cadre);
    sfRenderWindow_DrawString(Game, textbox->text);
}


// Widget bouton
Widget_bouton* widget_bouton_Create(sfIntRect rect, void (*f)(void*, void*), void* arg, void* arg2, sfImage* image_OnOver, sfImage* image_OnClick, sfImage* image_OnNothing)
{
    Widget_bouton *bouton = NULL;
    assert(bouton = (Widget_bouton*) malloc(sizeof(Widget_bouton)));

    bouton->rect = rect;
    bouton->onClick_Callback = f;
    bouton->onClick_Callback_arg = arg;
    bouton->onClick_Callback_arg2 = arg2;

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

    free_secure(bouton);
}

void widget_bouton_Click(Widget_bouton* bouton, int x, int y)
{
    if (sfIntRect_Contains(&bouton->rect, x, y))
        bouton->onClick_Callback(bouton->onClick_Callback_arg, bouton->onClick_Callback_arg2);
}

void widget_bouton_Over(Widget_bouton* bouton, int x, int y)
{
    if (sfIntRect_Contains(&bouton->rect, x, y))
        bouton->On = OVER;
    else
        bouton->On = NOTHING;
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


// Widget slide
Widget_slide* widget_slide_Create(int x, int y, int l, int h, int nbr_valeur, sfColor couleur, sfImage* s_top, sfImage* s_bottom, sfImage* s_middle)
{
    Widget_slide* slide = NULL;
    assert(slide = (Widget_slide*) malloc(sizeof(Widget_slide)));

    slide->sprite_top = sfSprite_Create();
    sfSprite_SetImage(slide->sprite_top, s_top);
    sfSprite_SetPosition(slide->sprite_top, x, y);
    sfSprite_Resize(slide->sprite_top, l, sfSprite_GetHeight(slide->sprite_top));

    slide->sprite_bottom = sfSprite_Create();
    sfSprite_SetImage(slide->sprite_bottom, s_bottom);
    sfSprite_SetPosition(slide->sprite_bottom, x, y+h-sfSprite_GetHeight(slide->sprite_bottom));
    sfSprite_Resize(slide->sprite_bottom, l, sfSprite_GetHeight(slide->sprite_bottom));

    slide->sprite_middle = sfSprite_Create();
    sfSprite_SetImage(slide->sprite_middle, s_middle);
    sfSprite_SetPosition(slide->sprite_middle, x, y+sfSprite_GetHeight(slide->sprite_top));
    sfSprite_Resize(slide->sprite_middle, l, (h-sfSprite_GetHeight(slide->sprite_bottom)-sfSprite_GetHeight(slide->sprite_top))/nbr_valeur);

    slide->x = x;
    slide->y = y;
    slide->largeur = l;
    slide->hauteur = h;

    slide->hauteur_step = (h-sfSprite_GetHeight(slide->sprite_bottom)-sfSprite_GetHeight(slide->sprite_top))/nbr_valeur;

    slide->couleur = couleur;

    slide->nombre_valeur = nbr_valeur;
    slide->valeur = 0;

    return slide;
}

void widget_slide_SetNbrVal(Widget_slide* slide, int nbr_valeur)
{

    slide->valeur = 0;
    sfSprite_SetPosition(slide->sprite_middle, slide->x, slide->y+sfSprite_GetHeight(slide->sprite_top));
    sfSprite_Resize(slide->sprite_middle, slide->largeur, (slide->hauteur-sfSprite_GetHeight(slide->sprite_bottom)-sfSprite_GetHeight(slide->sprite_top))/nbr_valeur);

    slide->hauteur_step = (slide->hauteur-sfSprite_GetHeight(slide->sprite_bottom)-sfSprite_GetHeight(slide->sprite_top))/nbr_valeur;

    slide->nombre_valeur = nbr_valeur;

}

void widget_slide_Destroy(Widget_slide* slide)
{
    sfSprite_Destroy(slide->sprite_top);
    sfSprite_Destroy(slide->sprite_bottom);
    sfSprite_Destroy(slide->sprite_middle);

    free_secure(slide);
}

void widget_slide_Draw(sfRenderWindow* Game, Widget_slide* slide)
{
    sfShape* shape = sfShape_CreateRectangle(slide->x, slide->y, slide->x+slide->largeur, slide->y+slide->hauteur, slide->couleur, 0, slide->couleur);

    sfRenderWindow_DrawShape(Game, shape);
    sfRenderWindow_DrawSprite(Game, slide->sprite_top);
    sfRenderWindow_DrawSprite(Game, slide->sprite_bottom);
    sfRenderWindow_DrawSprite(Game, slide->sprite_middle);

    sfShape_Destroy(shape);
}

void widget_slide_Click(Widget_slide* slide, int x, int y)
{
    sfIntRect rect = {sfSprite_GetX(slide->sprite_top), sfSprite_GetY(slide->sprite_top), sfSprite_GetX(slide->sprite_top)+sfSprite_GetWidth(slide->sprite_top), sfSprite_GetY(slide->sprite_top)+sfSprite_GetHeight(slide->sprite_top)};
    if (sfIntRect_Contains(&rect, x, y))
    {
        if (slide->valeur > 0)
        {
            slide->valeur--;
            sfSprite_Move(slide->sprite_middle, 0, -slide->hauteur_step);
        }

    }

    sfIntRect rect2 = {sfSprite_GetX(slide->sprite_bottom), sfSprite_GetY(slide->sprite_bottom), sfSprite_GetX(slide->sprite_bottom)+sfSprite_GetWidth(slide->sprite_bottom), sfSprite_GetY(slide->sprite_bottom)+sfSprite_GetHeight(slide->sprite_bottom)};
    if (sfIntRect_Contains(&rect2, x, y))
    {
        if (slide->valeur < slide->nombre_valeur-1)
        {
            slide->valeur++;
            sfSprite_Move(slide->sprite_middle, 0, slide->hauteur_step);
        }

    }
}


// Gui
Gui* gui_Create()
{
    Gui* gui = NULL;
    assert(gui = (Gui*)malloc(sizeof(Gui)));

    gui->widget_textbox = NULL;
    gui->widget_textbox_nombre = 0;

    gui->widget_bouton = NULL;
    gui->widget_bouton_nombre = 0;

    gui->widget_slide = NULL;
    gui->widget_slide_nombre = 0;

    return gui;
}

void gui_Load_Textbox(Gui* gui, Widget_textbox** widget, int taille)
{
    gui->widget_textbox_nombre = taille;
    gui->widget_textbox = widget;
}

void gui_Add_Textbox(Gui* gui, Widget_textbox* widget)
{
    assert(gui->widget_textbox = realloc(gui->widget_textbox, ++gui->widget_textbox_nombre * sizeof(Widget_textbox*)));
    gui->widget_textbox[gui->widget_textbox_nombre - 1] = widget;
}

bool gui_Exist_Textbox(Gui* gui, unsigned int id)
{
    return gui->widget_textbox_nombre > id;
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


void gui_Load_Slide(Gui* gui, Widget_slide** widget, int taille)
{
    gui->widget_slide_nombre = taille;
    gui->widget_slide = widget;
}

void gui_Add_Slide(Gui* gui, Widget_slide* widget)
{
    assert(gui->widget_slide = realloc(gui->widget_slide, ++gui->widget_slide_nombre * sizeof(Widget_slide*)));
    gui->widget_slide[gui->widget_slide_nombre - 1] = widget;
}

void gui_Destroy(Gui* gui)
{
    for (int i = 0; i < gui->widget_textbox_nombre; i++)
        widget_textbox_Destroy(gui->widget_textbox[i]);

    for (int i = 0; i < gui->widget_bouton_nombre; i++)
        widget_bouton_Destroy(gui->widget_bouton[i]);

    for (int i = 0; i < gui->widget_slide_nombre; i++)
        widget_slide_Destroy(gui->widget_slide[i]);

    free_secure(gui->widget_textbox);

    free_secure(gui->widget_bouton);

    free_secure(gui->widget_slide);

    free_secure(gui);
}

void gui_Draw(sfRenderWindow* Game, Gui* gui)
{
    for (int i = 0; i < gui->widget_textbox_nombre; i++)
        widget_textbox_Draw(Game, gui->widget_textbox[i]);

    for (int i = 0; i < gui->widget_bouton_nombre; i++)
        widget_bouton_Draw(Game, gui->widget_bouton[i]);

    for (int i = 0; i < gui->widget_slide_nombre; i++)
        widget_slide_Draw(Game, gui->widget_slide[i]);
}

void gui_Click(Gui* gui, int x, int y)
{
    for (int i = 0; i < gui->widget_textbox_nombre; i++)
        widget_textbox_Click(gui->widget_textbox[i], x, y);

    for (int i = 0; i < gui->widget_bouton_nombre; i++)
        widget_bouton_Click(gui->widget_bouton[i], x, y);

    for (int i = 0; i < gui->widget_slide_nombre; i++)
        widget_slide_Click(gui->widget_slide[i], x, y);
}

void gui_MouseOver(Gui* gui, int x, int y)
{

    for (int i = 0; i < gui->widget_bouton_nombre; i++)
        widget_bouton_Over(gui->widget_bouton[i], x, y);

}

void gui_TextEntered(Gui* gui, sfUint32 unicode)
{
    for (int i = 0; i < gui->widget_textbox_nombre; i++)
    {
        if (widget_textbox_IsActive(gui->widget_textbox[i]))
        {
            widget_textbox_Write(gui->widget_textbox[i], unicode);
        }
    }
}
