#ifndef GAMESCREENS_H
#define GAMESCREENS_H

#include <stdbool.h>
#include <SFML/Graphics.h>

void display_Menu(sfRenderWindow*);
_Bool display_PlayMenu(sfRenderWindow*, sfImage*, sfFont*);
_Bool display_ServerMenu(sfRenderWindow*, sfImage*, sfFont*);
_Bool display_ClientMenu(sfRenderWindow*, sfImage*, sfFont*);
_Bool display_Credits(sfRenderWindow*, sfImage*, sfFont*);
_Bool display_Settings(sfRenderWindow*, sfImage*, sfFont*);

#endif
