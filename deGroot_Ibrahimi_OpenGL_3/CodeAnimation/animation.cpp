#include "animation.h"

Animation::Animation(int duration) : resetFrame(duration) {}

Animation::~Animation() {}

void Animation::tick() {
    transformMat.setToIdentity();
    frame = (frame + 1) % resetFrame;
}
