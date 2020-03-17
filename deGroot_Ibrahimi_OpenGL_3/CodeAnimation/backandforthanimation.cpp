#include "backandforthanimation.h"

BackAndForthAnimation::BackAndForthAnimation(int duration, float speed, const QVector3D& direction)
    : Animation(duration), speed(speed), direction(direction) {

}

void BackAndForthAnimation::animate(Drawable* obj) {
    if (frame < resetFrame / 2) {
        obj->translate(direction * speed);
    } else {
        obj->translate(-direction * speed);
    }
}
