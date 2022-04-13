//
// Created by Nemo li on 2022/4/13.
//

#include "Scene.h"

Result Scene::threeEmissiveSphereScene(float x, float y)
{
    Result ret1 = { SDF::circleSDF(x,y,0.3f,0.3f,0.1f),glm::vec3(2.0f) };
    Result ret2 = { SDF::circleSDF(x,y,0.3f,0.7f,0.05f),glm::vec3(0.8f) };
    Result ret3 = { SDF::circleSDF(x,y,0.7f,0.5f,0.1f),glm::vec3(0.0f) };
    return SDF::unionOperation(SDF::unionOperation(ret1, ret2), ret3);
}