#ifndef ANIMATION_H
#define ANIMATION_H

#include <QMatrix4x4>

#include "drawable.h"

/**
 * @brief The Animation class
 * An abstract animation. Should be extended and implemented.
 */
class Animation
{
protected:
    int frame;
    int resetFrame;
    QMatrix4x4 transformMat;

public:
    Animation(int duration);
    virtual ~Animation();

    /**
     * @brief tick: advances the animation by 1 frame.
     */
    void tick();

    /**
     * @brief animate: transforms an object according to the current animation frame.
     */
    virtual void animate(Drawable* obj) = 0;
};

#endif // ANIMATION_H
