#ifndef SCREEN_H
#define SCREEN_H

#include "SFML/Graphics.h"
#include "SFML/Audio.h"

typedef struct SCREEN
{
    sfSprite* background;

    sfSprite** img_options;
    unsigned int nb_options;

    sfMusic* music;
} Screen;

Screen* screen_Create();
void screen_Destroy(Screen*);
int screen_LoadMusic(Screen*, sfMusic*, sfBool);
int screen_LoadBG(Screen*, sfImage*);

#endif
