#include <assert.h>
#include <stdio.h>
#include "Objects/Screen.h"

// Constructeur
Screen* screen_Create()
{
    Screen* new_screen = NULL;

    assert(new_screen = (Screen*) malloc(sizeof(Screen)));

    new_screen->images = NULL;
    new_screen->nb_img = 0;

    new_screen->texts = NULL;
    new_screen->nb_text = 0;

    new_screen->font = NULL;
    new_screen->music = NULL;

    return new_screen;
}

// Destructeur
void screen_Destroy(Screen* screen2destroy)
{
    assert(screen2destroy != NULL);

    for(int i = 0; i < screen2destroy->nb_text; i++)
        sfString_Destroy(screen2destroy->texts[i]);
    free(screen2destroy->texts);

    for(int i = 0; i < screen2destroy->nb_img; i++)
        sfSprite_Destroy(screen2destroy->images[i]);
    free(screen2destroy->images);

    sfFont_Destroy(screen2destroy->font);
    sfMusic_Destroy(screen2destroy->music);
    free(screen2destroy);
}

// Charge une police
void screen_LoadFont(Screen* screen, sfFont* font_)
{
    if(!font_)
    {
        printf("Warning - screen_LoadFont : sfFont object sent NULL\n");
        return;
    }

    screen->font = font_;
}

// Ajout d'une sfString dans Screen
void screen_LoadText(Screen* screen, char* text, sfColor color)
{
    assert(screen != NULL);

    screen->nb_text++;
    if(!screen->texts)
        assert(screen->texts = (sfString**) malloc(sizeof(sfString*)));
    else
        assert(screen->texts = (sfString**) realloc(screen->texts, screen->nb_text * sizeof(sfString*)));

    screen->texts[screen->nb_text - 1] = sfString_Create();
    sfString_SetText(screen->texts[screen->nb_text - 1], text);
    sfString_SetFont(screen->texts[screen->nb_text - 1], screen->font);
    sfString_SetColor(screen->texts[screen->nb_text - 1], color);
}

// Dessin de texte
void screen_DrawText(sfRenderWindow* window, Screen* screen, int id, int font_size, float x, float y)
{
    assert(screen != NULL && screen->texts != NULL);

    sfString_SetSize(screen->texts[id], font_size);
    sfString_SetX(screen->texts[id], x);
    sfString_SetY(screen->texts[id], y);
    sfRenderWindow_DrawString(window, screen->texts[id]);
}

// Charge une musique dans un Screen
void screen_LoadMusic(Screen* screen, sfMusic* music, sfBool loop)
{
    if(!music)
    {
        printf("Warning - screen_LoadMusic : sfMusic object doesn't exist\n");
        return;
    }

    screen->music = music;
    sfMusic_SetLoop(screen->music, loop);
}

// Lecture de la musique de l'écran donné
void screen_PlayMusic(Screen* screen)
{
    if(screen->music)
        sfMusic_Play(screen->music);
}

// Charge un arrière plan dans un Screen
void screen_LoadImage(Screen* screen, sfImage* image)
{
    assert(screen != NULL);

    screen->nb_img++;
    if(!screen->images)
        assert(screen->images = (sfSprite**) malloc(sizeof(sfSprite*)));
    else
        assert(screen->images = (sfSprite**) realloc(screen->images, screen->nb_img * sizeof(sfSprite*)));

    screen->images[screen->nb_img - 1] = sfSprite_Create();

    if(!image)
    {
        printf("Warning - screen_LoadImage : sfImage object sent NULL, skipping\n");
        return;
    }

    sfSprite_SetImage(screen->images[screen->nb_img - 1], image);
}
