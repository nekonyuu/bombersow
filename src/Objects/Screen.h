#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <Gui/Gui.h>

typedef struct SCREEN
{
    sfSprite** images;          // Sprites de l'écran
    unsigned int nb_img;        // Nombre d'images

    sfString** texts;           // Textes
    unsigned int nb_text;       // Nombre de textes

    Gui* gui;                   // Structure stockant les boutons et textbox

    sfFont* font;               // Police des options
    sfMusic* music;             // Musique
} Screen;

Screen* screen_Create();
void screen_Destroy(Screen*);
void screen_LoadFont(Screen*, sfFont*);
void screen_LoadText(Screen*, char*, sfColor, int, sfStringStyle, float, float);
void screen_DrawText(sfRenderWindow*, Screen*, int);
void screen_LoadMusic(Screen*, sfMusic*, sfBool);
void screen_PlayMusic(Screen*);
void screen_LoadImage(Screen*, sfImage*);
void screen_AddTextbox(Screen*, int, int, int, int, int, sfImage*, sfColor, Widget_textbox_type, void*, char*, sfColor, sfFont*, int);

#endif
