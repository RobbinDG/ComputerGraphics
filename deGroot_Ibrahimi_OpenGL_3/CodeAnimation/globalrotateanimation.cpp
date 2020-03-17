#include "globalrotateanimation.h"

#include <QMatrix4x4>

GlobalRotateAnimation::GlobalRotateAnimation(int duration, float frameangle, QVector3D axis)
    : Animation(duration), angle(frameangle), axis(axis)
{
}

void GlobalRotateAnimation::animate(Drawable* obj) {
    QMatrix4x4 mat;
    mat.setToIdentity();
    mat.rotate(frame * angle, axis);
    obj->transform(mat);
}
