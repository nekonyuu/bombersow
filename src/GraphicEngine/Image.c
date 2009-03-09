#include <assert.h>
#include "GraphicEngine/Image.h"

Image* image_Create(){
    Image* new_image = NULL;
    assert(new_image = (Image*) malloc(sizeof(Image)));

    new_image->image_nombre = 0;
    new_image->image_tab = NULL;

    return new_image;
}

void image_Destroy(Image *image_){
    int i = 0;
    assert(image_ != NULL);
    for(i = 0; i < image_->image_nombre; i++){
        sfImage_Destroy(image_->image_tab[i]);
    }
    free(image_->image_tab);
    free(image_);
}

void image_Loader(Image *image_, char **image_liste, int taille_liste){

    assert(image_->image_tab = (sfImage**) malloc(taille_liste*sizeof(sfImage*)));
    for(int i = 0; i < taille_liste; i++){
        image_->image_tab[i] = sfImage_CreateFromFile(image_liste[i]);
    }

    image_->image_nombre = taille_liste;
}

sfImage* image_Get(Image *image_, int id){
    if(id >= image_->image_nombre)
        return image_->image_tab[0];
    else
        if(image_->image_tab[id] != NULL)
            return image_->image_tab[id];
        else
            return image_->image_tab[0];

}


/*Animation* animation_Create(){
    Animation* animation_ = NULL;
    assert(animation_ = (Animation*) malloc(sizeof(Animation)));

    animation_->sprite = sfSprite_Create();
    animation_->x = 0;
    animation_->y = 0;
    animation_->image_hauteur = 0;
    animation_->image_largeur = 0;
    animation_->nombre_image = 0;
    animation_->cur_image = 0;
    animation_->play = STOP;
    animation_->fps = 0;
    animation_->clock = sfClock_Create();

    return animation_;
}*/

Animation* animation_Create(sfImage *image, float x, float y, int hauteur, int largeur, int nombre_image, int cur_image, int play, float fps){
    Animation* animation_ = NULL;
    assert(animation_ = (Animation*) malloc(sizeof(Animation)));

    animation_->sprite = sfSprite_Create();
    sfSprite_SetImage(animation_->sprite, image);
    animation_->x = x;
    animation_->y = y;
    animation_->image_hauteur = hauteur;
    animation_->image_largeur = largeur;
    animation_->nombre_image = nombre_image;
    animation_->cur_image = cur_image;
    animation_->play = play;
    animation_->fps = fps;
    animation_->clock = sfClock_Create();

    return animation_;
}


void animation_Destroy(Animation *animation_){
    sfSprite_Destroy(animation_->sprite);
    sfClock_Destroy(animation_->clock);
    free(animation_);
}

void animation_Play(Animation* animation_, int play){
    animation_->play = play;
    animation_->cur_image = 0;
    sfClock_Reset(animation_->clock);
}

void animation_Draw(Animation* animation_, sfRenderWindow* App){

    int time = sfClock_GetTime(animation_->clock);

    if(animation_->play != STOP && animation_->fps < time){
        animation_->play = (animation_->play != BOUCLE) ? animation_->play-- : animation_->play;
        animation_->cur_image = (animation_->cur_image+1 > animation_->nombre_image) ? 0 : animation_->cur_image+1;
        sfClock_Reset(animation_->clock);
    }

    sfIntRect rect = {animation_->x + animation_->image_largeur*animation_->cur_image, animation_->y, animation_->x + animation_->image_largeur*animation_->cur_image+animation_->image_largeur, animation_->y + animation_->image_hauteur};
    sfSprite_SetSubRect(animation_->sprite, &rect);
    sfRenderWindow_DrawSprite(App, animation_->sprite);
}


