#include "animation.h"

Animation::Animation(int duration) : frame(0), resetFrame(duration) {}

Animation::~Animation() {}

void Animation::tick() {
    transformMat.setToIdentity();
    frame = (frame + 1) % resetFrame;
}
