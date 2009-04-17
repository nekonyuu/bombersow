#include "BaseSystem/Logging.h"
#include "Game/MenuScreen.h"
#include "Game/PlayScreen.h"
#include "Game/SettingsScreen.h"
#include "Game/CreditsScreen.h"
#include "GraphicEngine/Draw.h"
#include "Objects/Screen.h"
#include "Objects/GameObjects.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "PhysicsEngine/CollisionSystem.h"
#include "PhysicsEngine/GravitySystem.h"

void display_Menu(sfRenderWindow* Game)
{
    Screen* Menu = screen_Create();
    sfMusic *menuMusic = sfMusic_CreateFromFile("sounds/music/ParagonX9 - Metropolis [8-Bit].ogg");
    sfImage *BG_image = sfImage_CreateFromFile("base/images/Menu/menu_bg.png");
    sfImage *image_animation = sfImage_CreateFromFile("base/images/animation.png"); // Test
    sfFont *menuFont = sfFont_CreateFromFile("base/fonts/ITCKRIST.TTF", 50, NULL);
    Animation *animation = animation_Create(image_animation, 0, 0, 30, 30, 4, 0, BOUCLE, 0.1f);
    Animation *animation2 = animation_Create(image_animation, 0, 0, 30, 30, 4, 0, BOUCLE, 0.1f);
    sfEvent Event;
    _Bool launched = true;
    int menu_select = 1;

    screen_LoadImage(Menu, BG_image);                           // Chargement de l'arri�re-plan
    screen_LoadFont(Menu, menuFont);                            // Chargement de la police d'�criture
    // Pr�paration des textes
    screen_LoadText(Menu, GAME_NAME, sfRed, 60, sfStringRegular, 450.0f, 40.0f);
    screen_LoadText(Menu, "Jouer", sfRed, 35, sfStringItalic, 450.0f, 140.0f);
    screen_LoadText(Menu, "Options", sfWhite, 35, sfStringItalic, 450.0f, 190.0f);
    screen_LoadText(Menu, "Credits", sfWhite, 35, sfStringItalic, 450.0f, 240.0f);
    screen_LoadText(Menu, "Quitter", sfWhite, 35, sfStringItalic, 450.0f, 290.0f);
    screen_LoadMusic(Menu, menuMusic, sfTrue);                  // Chargement de la musique
    if (Menu->music)
        screen_PlayMusic(Menu);                                 // Lecture

    logging_Info("display_Menu", "Started without error");

    Object* obj_temp = object_Create(0);
    object_LoadImg(obj_temp, NULL, animation);
    object_SetPosition(obj_temp, 0, 560);

    Object* obj_temp2 = object_Create(0);
    object_LoadImg(obj_temp2, NULL, animation2);
    object_SetPosition(obj_temp2, 0, 230);

    Map* map = map_Create(0, 0);
    map_AddObject(map, obj_temp);

    Player* player_tab[100];

    Animation* animation3 = NULL;
    Sprite* spr = NULL;
    for(int i = 0; i < 6; i++)
    {

        spr = sprite_Create(0, 0, image_animation, NULL);

        player_tab[i] = player_Create("a", 0);
        player_tab[i]->sprite = spr;
        player_SetPosition(player_tab[i], i*130, 0);
        map_AddPlayer(map, player_tab[i]);
    }

    do
    {
        sfRenderWindow_Clear(Game, sfBlack);                    // Vidage de l'�cran

        sfRenderWindow_DrawSprite(Game, Menu->images[0]);       // Dessin du BG

        for (int i = 0; i < Menu->nb_text; i++)
            screen_DrawText(Game, Menu, i);                     // Dessin des textes

        gravitysystem_WorldUpdate(map, 9.81);
        map_Draw(Game, map);

        quad_tree_Draw(Game, map->quad_tree);

        sfRenderWindow_Display(Game);                           // Mise � jour de la fen�tre

        while (sfRenderWindow_GetEvent(Game, &Event))           // Surveillance des �v�nements
        {
            switch (Event.Type)
            {
            case sfEvtClosed:                                   // Fermer : Quitter le jeu
                launched = false;
                break;

            case sfEvtKeyPressed:
                switch (Event.Key.Code)
                {
                case sfKeyEscape:                               // Quitte le jeu
                    launched = false;
                    break;

                case sfKeyReturn:
                    switch (menu_select)
                    {
                    case 1:                                     // Affichage du menu de jeu
                        if (display_PlayMenu(Game, BG_image, menuFont))
                            launched = false;
                        break;
                    case 2:                                     // Affichage du menu options
                        if (display_Settings(Game, BG_image, menuFont))
                            launched = false;
                        break;
                    case 3:                                     // Affichage des credits
                        if (display_Credits(Game, BG_image, menuFont))
                            launched = false;
                        break;
                    case 4:                                     // Quitter
                        launched = false;
                        break;
                    }
                    break;

                case sfKeyUp:
                    if (menu_select > 1)
                    {
                        sfString_SetColor(Menu->texts[menu_select], sfWhite);
                        sfString_SetColor(Menu->texts[--menu_select], sfRed);
                    }
                    player_tab[0]->speed_y = -5;
                    break;

                case sfKeyDown:
                    if (menu_select < 4)
                    {
                        sfString_SetColor(Menu->texts[menu_select], sfWhite);
                        sfString_SetColor(Menu->texts[++menu_select], sfRed);
                    }
                    break;

                case sfKeyRight:
                    player_tab[0]->coord_x += 5;
                    break;

                default:
                    break;
                }
            default:
                break;
            }
        }

    }
    while (launched);

    map_Destroy(map);

    sfFont_Destroy(menuFont);
    sfImage_Destroy(BG_image);
    sfImage_Destroy(image_animation);
    screen_Destroy(Menu);

    return;
}
