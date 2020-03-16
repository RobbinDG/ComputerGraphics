#include "localrotateanimation.h"

LocalRotateAnimation::LocalRotateAnimation(int duration, float frameangle, QVector3D axis)
    : Animation(duration), angle(frameangle), axis(axis)
{}

void LocalRotateAnimation::animate(Drawable* obj) {
    obj->setRotation(frame * angle * axis);
}
