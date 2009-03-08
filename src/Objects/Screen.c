#include <assert.h>
#include "Objects/Screen.h"

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

void screen_Destroy(Screen* screen2destroy)
{
    sfSprite_Destroy(screen2destroy->background);
    for(int i = 0; i < screen2destroy->nb_options; i++)
        sfSprite_Destroy(screen2destroy->img_options[i]);
    sfMusic_Destroy(screen2destroy->music);
    free(screen2destroy);
}

int screen_LoadMusic(Screen* screen, sfMusic* music, sfBool loop)
{
    if (!music)
        return EXIT_FAILURE;

    screen->music = music;
    sfMusic_SetLoop(screen->music, loop);
    sfMusic_Play(screen->music);

    return EXIT_SUCCESS;
}

int screen_LoadBG(Screen* screen, sfImage* BG)
{
    if (!BG)
        return EXIT_FAILURE;

    screen->background = sfSprite_Create();
    sfSprite_SetImage(screen->background, BG);

    return EXIT_SUCCESS;
}
