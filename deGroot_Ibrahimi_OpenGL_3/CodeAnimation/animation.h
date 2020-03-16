#ifndef ANIMATION_H
#define ANIMATION_H

#include <QMatrix4x4>

#include "drawable.h"

class Animation
{
protected:
    int frame;
    int resetFrame;
    QMatrix4x4 transformMat;

public:
    Animation(int duration);
    virtual ~Animation();
    void tick();
    virtual void animate(Drawable* obj) = 0;
};

#endif // ANIMATION_H
