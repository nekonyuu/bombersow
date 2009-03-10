#ifndef SCREEN_H
#define SCREEN_H

#include "SFML/Graphics.h"
#include "SFML/Audio.h"

typedef struct SCREEN
{
    sfSprite** images;          // Sprites des Options
    unsigned int nb_img;        // Nombre d'options

    sfString** texts;           // Textes
    unsigned int nb_text;       // Nombre de textes

    sfFont* font;               // Police des options
    sfMusic* music;             // Musique
} Screen;

Screen* screen_Create();
void screen_Destroy(Screen*);
void screen_LoadFont(Screen*, sfFont*);
void screen_LoadText(Screen*, char*, sfColor);
void screen_DrawText(sfRenderWindow*, Screen*, int, int, float, float);
void screen_LoadMusic(Screen*, sfMusic*, sfBool);
void screen_PlayMusic(Screen*);
void screen_LoadImage(Screen*, sfImage*);

#endif
