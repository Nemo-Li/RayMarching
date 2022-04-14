//
// Created by Nemo li on 2022/4/13.
//

#include "Scene.h"

#define M_2PI 6.28318530718f

Result Scene::threeEmissiveSphereScene(float x, float y) {
    Result ret1 = {SDF::circleSDF(x, y, 0.3f, 0.3f, 0.1f), glm::vec3(2.0f)};
    Result ret2 = {SDF::circleSDF(x, y, 0.3f, 0.7f, 0.05f), glm::vec3(0.8f)};
    Result ret3 = {SDF::circleSDF(x, y, 0.7f, 0.5f, 0.1f), glm::vec3(0.0f)};
    return SDF::unionOperation(SDF::unionOperation(ret1, ret2), ret3);
}

Result Scene::moonEmissiveScene(float x, float y) {
    Result ret1 = {SDF::circleSDF(x, y, 0.4f, 0.5f, 0.20f), glm::vec3(1.0f)};
    Result ret2 = {SDF::circleSDF(x, y, 0.6f, 0.5f, 0.20f), glm::vec3(0.8f)};
//    return SDF::unionOperation(ret1, ret2);
//    return SDF::intersectOperation(ret1, ret2);
    return SDF::subtractOperation(ret1, ret2);
}

Result Scene::sampleReflectScene(float x, float y) {
    Result a = {SDF::circleSDF(x, y, 0.4f, 0.2f, 0.1f), glm::vec3(2.0f), 0.0f};
    Result b = {SDF::boxSDF(x, y, 0.5f, 0.8f, M_2PI / 16.0f, 0.1f, 0.1f), glm::vec3(0.0f), 0.9f};
    Result c = {SDF::boxSDF(x, y, 0.8f, 0.5f, M_2PI / 16.0f, 0.1f, 0.1f), glm::vec3(0.0f), 0.9f};
    return SDF::unionOperation(SDF::unionOperation(a, b), c);
}