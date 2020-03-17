#ifndef BACKANDFORTHANIMATION_H
#define BACKANDFORTHANIMATION_H

#include "drawable.h"
#include "animation.h"

class BackAndForthAnimation : public Animation
{
private:
    float speed;
    QVector3D direction;

public:
    BackAndForthAnimation(int duration, float speed, const QVector3D& direction);
    void animate(Drawable* obj) override;
};

#endif // BACKANDFORTHANIMATION_H
