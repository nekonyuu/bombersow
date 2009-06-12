/*
    GPL v3 Licence :
    Bombersow is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bombersow is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bombersow.  If not, see <http://www.gnu.org/licenses/>.


    Creative Commons BY-NC-SA :
    This work is licensed under the Creative Commons Attribution-Noncommercial-Share Alike 3.0 Unported License.
    To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter
    to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.

*/

#include <BaseSystem/Logging.h>
#include <Objects/GameObjects.h>

PlayersList* playerslist_Create(Map* map, sfFont* font, sfColor color, int font_size, sfStringStyle style, float base_x, float base_y)
{
    PlayersList* ptr = (PlayersList*) malloc(sizeof(PlayersList));

    ptr->players = (sfString**) malloc(map->max_players * sizeof(sfString*));
    ptr->max_players = map->max_players;
    ptr->nb_players = 0;

    for (unsigned int i = 0; i < map->max_players; i++)
    {
        ptr->players[i] = sfString_Create();
        sfString_SetColor(ptr->players[i], color);
        sfString_SetFont(ptr->players[i], font);
        sfString_SetSize(ptr->players[i], font_size);
        sfString_SetStyle(ptr->players[i], style);
        sfString_SetPosition(ptr->players[i], base_x, base_y + i * (font_size + 5));
        if (map->players_list[i])
        {
            sfString_SetText(ptr->players[i], map->players_list[i]->char_name);
            ptr->nb_players++;
        }
    }

    return ptr;
}

void playerslist_Destroy(PlayersList* ptr)
{
    for (unsigned int i = 0; i < ptr->max_players; i++)
        sfString_Destroy(ptr->players[i]);

    free_secure(ptr->players);
    free_secure(ptr);
}

void playerslist_Draw(PlayersList* ptr, sfRenderWindow* App)
{
    if (!ptr)
    {
        logging_Warning("playerslist_Draw", "PlayersList object sent NULL");
        return;
    }

    for (unsigned int i = 0; i < ptr->nb_players; i++)
        sfRenderWindow_DrawString(App, ptr->players[i]);
}

void playerslist_Update(PlayersList* lst, Map* map)
{
    for (lst->nb_players = 0; lst->nb_players < map->nb_players; lst->nb_players++)
        sfString_SetText(lst->players[lst->nb_players], map->players_list[lst->nb_players]->char_name);

    for (unsigned int i = lst->nb_players; i < lst->max_players; i++)
        sfString_SetText(lst->players[i], "");
}
