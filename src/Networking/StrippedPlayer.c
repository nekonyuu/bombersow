#include <stdio.h>
#include <assert.h>
#include "Objects/Player.h"
#include "Networking/StrippedPlayer.h"

StPlayer* stplayer_Create(Player* player_)
{
    StPlayer* new_stplayer;
    assert(new_stplayer = (StPlayer*) malloc(sizeof(StPlayer)));

    new_stplayer->name = player_->name;

    new_stplayer->current_weapon = &player_->current_weapon;

    new_stplayer->coord_x = &player_->coord_x;
    new_stplayer->coord_y = &player_->coord_y;

    return new_stplayer;
}

void stplayer_Destroy(StPlayer* stplayer_)
{
    if(!stplayer_)
    {
        printf("Warning - stplayer_Destroy : StPlayer object sent NULL\n");
        return;
    }
    stplayer_->name = NULL;
    stplayer_->current_weapon = NULL;
    stplayer_->coord_x = NULL;
    stplayer_->coord_y = NULL;

    free(stplayer_);
}
