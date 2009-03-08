#ifndef SCREEN_H
#define SCREEN_H

#include "SFML/Graphics.h"
#include "SFML/Audio.h"

typedef struct SCREEN
{
    sfSprite* background;       // Arrière-plan

    sfSprite** img_options;     // Sprites des Options
    unsigned int nb_options;    // Nombre d'options

    sfMusic* music;             // Musique
} Screen;

Screen* screen_Create();
void screen_Destroy(Screen*);
void screen_LoadMusic(Screen*, sfMusic*, sfBool);
void screen_PlayMusic(Screen*);
void screen_LoadBG(Screen*, sfImage*);

#endif
