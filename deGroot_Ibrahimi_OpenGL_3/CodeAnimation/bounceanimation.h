#ifndef BOUNCEANIMATION_H
#define BOUNCEANIMATION_H

#include "animation.h"

class BounceAnimation : public Animation
{
private:
    float velocity, acceleration;
    QVector3D direction;

public:
    BounceAnimation(int duration, float height, QVector3D direction);
    void animate(Drawable* obj) override;
};

#endif // BOUNCEANIMATION_H
