#ifndef LOCALROTATEANIMATION_H
#define LOCALROTATEANIMATION_H

#include "animation.h"

class LocalRotateAnimation : Animation
{
protected:
    float angle;
    QVector3D axis;

public:
    LocalRotateAnimation(int duration, float frameangle, QVector3D axis);
    void animate(Drawable* obj) override;
};

#endif // LOCALROTATEANIMATION_H
