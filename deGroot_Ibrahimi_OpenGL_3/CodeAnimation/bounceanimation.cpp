#include "bounceanimation.h"

BounceAnimation::BounceAnimation(int duration, float height, QVector3D direction)
    : Animation(duration), velocity(0.0f), acceleration(height / ((duration / 2.0) * (duration / 2.0))), direction(direction)
{

}

void BounceAnimation::animate(Drawable* obj) {
    if (frame < resetFrame / 2) {
        obj->translate(-direction * velocity);
        velocity += acceleration;
    } else {
        obj->translate(direction * velocity);
        velocity -= acceleration;
    }
}
