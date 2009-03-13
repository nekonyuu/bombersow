#include <assert.h>
#include <string.h>
#include "SFML/Graphics.h"
#include "BaseSystem/Logging.h"
#include "Gui/Gui.h"

#define UNI32_RETURN 8


/*typedef enum CADRE {TOP, LEFT, RIGHT, BOTTOM, TL, TR, BL, BR} cadre;

typedef struct WIDGET_CADRE
{

    sfSprite *sprite_cadre[8];
    sfSprite *background;
    int x;
    int y;
    int width;
    int height;

} Widget_cadre;*/

Widget_cadre* widget_cadre_Create(sfImage* image, sfSprite* sprite_background, int x, int y, int width, int height){

    Widget_cadre* cadre = NULL;
    assert(cadre = (Widget_cadre*) malloc(sizeof(Widget_cadre)));

    for(int i = 0; i < 8; i++){
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
    if(!cadre)
    {
        logging_Warning("widget_cadre_Destroy", "Widget_cadre object sent NULL");
        return;
    }

    for(int i = 0; i < 8; i++){
        sfSprite_Destroy(cadre->sprite_cadre[i]);
    }

    sfSprite_Destroy(cadre->background);

    free(cadre);
}

void widget_cadre_Draw(sfRenderWindow* Game, Widget_cadre* cadre)
{
    assert(Game && cadre);

    sfRenderWindow_DrawSprite(Game, cadre->background);
    for(int i = 0; i < 8; i++)
        sfRenderWindow_DrawSprite(Game, cadre->sprite_cadre[i]);
}

// Construit la Widget_textbox_var
Widget_textbox_var* widget_text_box_var_Create(Widget_textbox_type type, void* var)
{
    Widget_textbox_var *textbox_var = NULL;
    assert(textbox_var = (Widget_textbox_var*) malloc(sizeof(Widget_textbox_var)));

    switch(type)
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
    if(!textbox_var)
    {
        logging_Warning("widget_textbox_var_Destroy", "Widget_textbox_var object sent NULL");
        return;
    }

    if(textbox_var->var_char)
        free(textbox_var->var_char);
    if(textbox_var->var_int)
        free(textbox_var->var_int);
    if(textbox_var->var_string)
        sfString_Destroy(textbox_var->var_string);

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
    switch(textbox_var->type)
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
    widget_cadre_Draw(Game, textbox->cadre);
    sfRenderWindow_DrawString(Game, textbox->text);
}
