#include <assert.h>
#include <string.h>
#include "SFML/Graphics.h"
#include "Gui/Gui.h"

#define UNI32_RETURN 8

Widget_textbox_var* widget_text_box_var_Create(Widget_textbox_type type, void* var)
{
    Widget_textbox_var *textbox_var = NULL;
    assert(textbox_var = (Widget_textbox_var*) malloc(sizeof(Widget_textbox_var)));

    if (type == INT)
    {
        textbox_var->type = INT;
        textbox_var->var_int = var;
        textbox_var->var_char = NULL;
        textbox_var->var_string = NULL;
    }
    else if (type == CHAR)
    {
        textbox_var->type = CHAR;
        textbox_var->var_char = var;
        textbox_var->var_int = NULL;
        textbox_var->var_string = NULL;
    }
    else if (type == STRING)
    {
        textbox_var->type = STRING;
        textbox_var->var_char = NULL;
        textbox_var->var_int = NULL;
        textbox_var->var_string = var;
    }

    return textbox_var;
}

void widget_textbox_var_Destroy(Widget_textbox_var* textbox_var)
{
    if (textbox_var != NULL)
    {
        textbox_var->var_char = NULL;
        textbox_var->var_int = NULL;
        textbox_var->var_string = NULL;
        free(textbox_var);
        textbox_var = NULL;

    }
}

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
    if (textbox_var->type == INT)
    {
        *textbox_var->var_int = atoi(textbox->text_char);
    }
    else if (textbox_var->type == CHAR)
    {
        strcpy(textbox_var->var_char, textbox->text_char);
    }
    else
    {
        textbox_var->var_string = textbox->text; //A CORRIGER
    }
}


Widget_textbox* widget_textbox_Create(int x, int y, int width, int height, int taille, sfImage *image_cadre, Widget_textbox_type type, void* var)
{
    Widget_textbox* textbox = NULL;
    assert(textbox = malloc(sizeof(Widget_textbox)));

    textbox->text = sfString_Create();
    sfString_SetPosition(textbox->text, x, y);
    sfString_SetColor(textbox->text, sfBlack);

    textbox->text_char = NULL;
    assert(textbox->text_char = (char*) malloc(taille*sizeof(char)));

    textbox->taille = taille;

    textbox->x = x;
    textbox->y = y;
    textbox->width = width;
    textbox->height = height;

    textbox->cadre = sfSprite_Create();
    sfSprite_SetImage(textbox->cadre, image_cadre);
    sfSprite_SetPosition(textbox->cadre, x, y);
    sfSprite_Resize(textbox->cadre, width, height);

    textbox->var = widget_text_box_var_Create(type, var);
    widget_textbox_var_Get(textbox->var, textbox);
    printf("%s\n", textbox->text_char);

    textbox->active = 0;

    return textbox;
}

void widget_textbox_Destroy(Widget_textbox* textbox)
{
    widget_textbox_var_Destroy(textbox->var);

    sfString_Destroy(textbox->text);

    sfSprite_Destroy(textbox->cadre);

    free(textbox->text_char);
    textbox->text_char = NULL;

    free(textbox);
    textbox = NULL;
}

void widget_textbox_Click(Widget_textbox* textbox, int x, int y)
{
    sfIntRect cadre_screen = {textbox->x, textbox->y, textbox->x+textbox->width, textbox->y+textbox->height};
    if (sfIntRect_Contains(&cadre_screen, x, y))
    {
        textbox->active = 1;
    }
    else
    {
        textbox->active = 0;
    }
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
    if (textbox->active == 1)
        return 1;
    else
        return 0;
}

void widget_textbox_Draw(sfRenderWindow* Game, Widget_textbox* textbox)
{
    sfRenderWindow_DrawShape(Game, textbox->cadre);
    sfRenderWindow_DrawString(Game, textbox->text);

    sfFloatRect *rect = sfString_GetRect(textbox->text);

    sfShape *temp = sfShape_CreateLine(rect->Right+2, rect->Top, rect->Right+2, rect->Bottom, 2, sfBlack, 0, sfBlack);
    sfRenderWindow_DrawShape(Game, temp);
    sfShape_Destroy(temp);
}
