#ifndef SCENE_H
#define SCENE_H

#include "mesh.h"

#include <vector>
#include <QOpenGLFunctions_3_3_Core>


class Scene
{
private:
    std::vector<Mesh> meshes;

public:
    Scene();
    void addMesh(Mesh& mesh);
};

#endif // SCENE_H
