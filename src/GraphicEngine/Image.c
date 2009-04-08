#include <assert.h>
#include "BaseSystem/Logging.h"
#include "GraphicEngine/Image.h"

// Constructeur
Image* image_Create()
{
    Image* new_image = NULL;
    assert(new_image = (Image*) malloc(sizeof(Image)));

    new_image->image_nombre = 0;
    new_image->image_tab = NULL;

    return new_image;
}

// Destructeur
void image_Destroy(Image *image_)
{
    int i = 0;
    if (!image_)
    {
        logging_Warning("image_Destroy", "Image object sent NULL");
    }
    else
    {
        for (i = 0; i < image_->image_nombre; i++)
            sfImage_Destroy(image_->image_tab[i]);

        free(image_->image_tab);
        free(image_);
    }
}

// Chargement d'une liste d'images dans un "objet" Image
void image_Loader(Image *image_, char **image_liste, int taille_liste)
{

    assert(image_->image_tab = (sfImage**) malloc(taille_liste*sizeof(sfImage*)));
    for (int i = 0; i < taille_liste; i++)
    {
        image_->image_tab[i] = sfImage_CreateFromFile(image_liste[i]);
    }

    image_->image_nombre = taille_liste;

}

// Acquisition d'une image suivant son id dans l'objet Image
sfImage* image_Get(Image *image_, int id)
{
    if (id >= image_->image_nombre)
        return image_->image_tab[0];
    else
        if (image_->image_tab[id] != NULL)
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

// Constructeur
Animation* animation_Create(sfImage *image, int x, int y, int hauteur, int largeur, int nombre_image, int cur_image, int play, float fps)
{
    Animation* animation_ = NULL;
    assert(animation_ = (Animation*) malloc(sizeof(Animation)));

    animation_->sprite = sfSprite_Create();
    sfSprite_SetImage(animation_->sprite, image);
    animation_->x = x;
    animation_->y = y;
    animation_->image_hauteur = hauteur;
    animation_->image_largeur = largeur;
    animation_->nombre_image = nombre_image-1;
    animation_->cur_image = cur_image;
    animation_->play = play;
    animation_->fps = fps;
    animation_->clock = sfClock_Create();

    animation_->x_c = 0;
    animation_->y_c = 0;

    //printf("%f",animation_->fps);

    return animation_;
}

// Destructeur
void animation_Destroy(Animation *animation_)
{
    if (animation_ != NULL)
    {
        if (animation_->sprite != NULL)
            sfSprite_Destroy(animation_->sprite);
        sfClock_Destroy(animation_->clock);
        free(animation_);
    }
}

//Set position
void animation_SetPosition(Animation* animation, int x, int y)
{

    animation->x_c = x;
    animation->y_c = y;
    sfSprite_SetPosition(animation->sprite, x, y);

}

// Lecture
void animation_Play(Animation* animation_, int play)
{
    animation_->play = play;
    animation_->cur_image = 0;
    sfClock_Reset(animation_->clock);
}

// Dessin de l'animation sur la fenêtre
void animation_Draw(Animation* animation_, sfRenderWindow* App)
{

    float time = sfClock_GetTime(animation_->clock);

    if (animation_->play != STOP && animation_->fps < time)
    {
        animation_->play = (animation_->play != BOUCLE) ? animation_->play-- : animation_->play;
        animation_->cur_image = (animation_->cur_image+1 > animation_->nombre_image) ? 0 : animation_->cur_image+1;
        sfClock_Reset(animation_->clock);
    }

    sfIntRect rect = {animation_->x + animation_->image_largeur*animation_->cur_image, animation_->y, animation_->x + animation_->image_largeur*animation_->cur_image+animation_->image_largeur, animation_->y + animation_->image_hauteur};
    sfSprite_SetSubRect(animation_->sprite, &rect);
    sfRenderWindow_DrawSprite(App, animation_->sprite);
}

//Sprite
Sprite* sprite_Create(int x, int y, sfImage* image, Animation* animation)
{

    Sprite* sprite;
    assert(sprite = (Sprite*) malloc(sizeof(Sprite)));

    sprite->x = x;
    sprite->y = y;

    sprite->sprite = NULL;
    sprite->animation = NULL;

    if (animation != NULL)
    {
        sprite->animation = animation;
        sprite->type = ANIMATION;

        sprite->hauteur = animation->image_hauteur;
        sprite->largeur = animation->image_largeur;

    }
    else if (image != NULL)
    {
        sprite->sprite = sfSprite_Create();
        sfSprite_SetImage(sprite->sprite, image);
        sprite->type = SPRITE;

        sprite->hauteur = sfImage_GetHeight(image);
        sprite->largeur = sfImage_GetWidth(image);
    }

    sprite_SetPosition(sprite, x, y);

    return sprite;
}

void sprite_Destroy(Sprite* sprite)
{
    if (!sprite)
    {
        logging_Warning("sprite_Destroy", "Sprite object sent NULL");
        return;
    }

    switch (sprite->type)
    {
    case SPRITE:
        sfSprite_Destroy(sprite->sprite);
        break;

    case ANIMATION:
        animation_Destroy(sprite->animation);
        break;
    }

    free(sprite);
    sprite = NULL;
}

void sprite_SetPosition(Sprite* sprite, int x, int y)
{

    sprite->x = x;
    sprite->y = y;

    if (sprite->type)
    {
        animation_SetPosition(sprite->animation, x , y);
    }
    else
    {
        sfSprite_SetPosition(sprite->sprite, (float)x, (float)y);
    }

}

void sprite_Draw(sfRenderWindow* Game, Sprite* sprite)
{

    if (sprite->type)
    {
        animation_Draw(sprite->animation, Game);
    }
    else
    {
        sfRenderWindow_DrawSprite(Game, sprite->sprite);
    }

}

void sprite_SetColor(Sprite* sprite, sfColor color)
{

    if (sprite->type)
    {
        sfSprite_SetColor(sprite->animation->sprite, color);
    }
    else
    {
        sfSprite_SetColor(sprite->sprite, color);
    }

}

sfImage* sprite_GetImage(Sprite* sprite)
{

    if (sprite->type)
    {
        return sfSprite_GetImage(sprite->animation->sprite);
    }
    else
    {
        return sfSprite_GetImage(sprite->sprite);
    }

}

sfIntRect sprite_GetRect(Sprite* sprite)
{
    sfIntRect rect = {sprite->x, sprite->y, sprite->x+sprite->largeur, sprite->y+sprite->hauteur};
    return rect;
}
