#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include "MenuScreen.h"

int display_Menu()
{
    if(startmusic_Menu() == EXIT_FAILURE)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int startmusic_Menu()
{
    sfMusic* menuMusic;

    // Chargement de la musique du menu et lecture en boucle
    menuMusic = sfMusic_CreateFromFile("sounds/music/ParagonX9 - Defcon Zero.ogg");
    if (!menuMusic)
        return EXIT_FAILURE;

    sfMusic_SetLoop(menuMusic, sfTrue);
    sfMusic_Play(menuMusic);

    return EXIT_SUCCESS;
}
