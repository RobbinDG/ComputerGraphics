#ifndef GLOBALROTATEANIMATION_H
#define GLOBALROTATEANIMATION_H

#include "animation.h"

class GlobalRotateAnimation : public Animation
{
protected:
    float angle;
    QVector3D axis;

public:
    GlobalRotateAnimation(int duration, float frameangle, QVector3D axis);
    void animate(Drawable* obj) override;
};

#endif // GLOBALROTATEANIMATION_H
