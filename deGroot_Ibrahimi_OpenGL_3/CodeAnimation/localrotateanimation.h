#ifndef LOCALROTATEANIMATION_H
#define LOCALROTATEANIMATION_H

#include "animation.h"
#include <QVector3D>

class LocalRotateAnimation : public Animation
{
protected:
    float angle;
    QVector3D axis;

public:
    LocalRotateAnimation(int duration, float frameangle, QVector3D axis);
    void animate(Drawable* obj) override;
};

#endif // LOCALROTATEANIMATION_H
