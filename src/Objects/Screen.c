#include <assert.h>
#include "Objects/Screen.h"

// Constructeur
Screen* screen_Create()
{
    Screen* new_screen = NULL;

    assert(new_screen = (Screen*) malloc(sizeof(Screen)));

    new_screen->background = NULL;
    new_screen->nb_options = 0;
    new_screen->img_options = NULL;
    new_screen->music = NULL;

    return new_screen;
}

// Destructeur
void screen_Destroy(Screen* screen2destroy)
{
    assert(screen2destroy != NULL);

    sfSprite_Destroy(screen2destroy->background);
    for(int i = 0; i < screen2destroy->nb_options; i++)
        sfSprite_Destroy(screen2destroy->img_options[i]);
    sfMusic_Destroy(screen2destroy->music);
    free(screen2destroy);
}

// Charge une musique dans un Screen
void screen_LoadMusic(Screen* screen, sfMusic* music, sfBool loop)
{
    assert(music != NULL);

    screen->music = music;
    sfMusic_SetLoop(screen->music, loop);
}

void screen_PlayMusic(Screen* screen)
{
    assert(screen->music != NULL);

    sfMusic_Play(screen->music);
}

// Charge un arrière plan dans un Screen
void screen_LoadBG(Screen* screen, sfImage* BG)
{
    assert(BG != NULL);

    screen->background = sfSprite_Create();
    sfSprite_SetImage(screen->background, BG);
}
