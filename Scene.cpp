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

Result Scene::beerLambertScene(float x, float y)
{
    Result a = { SDF::circleSDF(x, y, 0.3f, -0.2f, 0.1f), glm::vec3(10.0f),
                 0.0f, 0.0f, glm::vec3(0.0f) };
    Result b = { SDF::ngonSDF(x, y, 0.5f, 0.5f, 0.25f, 8.0f), glm::vec3(0.0f),
                 0.0f, 1.5f, glm::vec3(4.0f, 1.0f, 4.0f)};
    return SDF::unionOperation(a, b);
}

Result Scene::rectRefractEmissiveScene(float x, float y)
{
    Result ret1 = Result(SDF::boxSDF(x, y, 0.5f, 0.5f, 0.0f, 0.3f, 0.2f), glm::vec3(0.0f), 0.2f, 1.5f, glm::vec3(4.0f, 4.0f, 1.0f));
    Result ret2 = Result(SDF::circleSDF(x, y, 0.1f, 0.1f, 0.1f), glm::vec3(7.0f), 0.0f, 0.0f);
    return SDF::unionOperation(ret1, ret2);
}

Result Scene::WScene(float x, float y) {
    Result light = Result(SDF::circleSDF(x, y, 0.8f, 0.85f, 0.025f), glm::vec3(8.0), 0.0f);
    Result wc = Result(SDF::WcharSDF(x, y, 0.35 + 0.05, 0.55, 0.01f), glm::vec3(0.0f, 3.5f, 0.0f), 0.0f);
    return SDF::unionOperation(light, wc);
}

Result Scene::WLScene(float x, float y) {
    Result light = Result(SDF::circleSDF(x, y, 0.8f, 0.85f, 0.025f), glm::vec3(8.0), 0.0f);
    Result wc = Result(SDF::WcharSDF(x, y, 0.40 + 0.03, 0.55, 0.01f), glm::vec3(0.0f, 3.5f, 0.0f), 0.0f);
    Result lc = Result(SDF::LcharSDF(x, y, 0.55 + 0.03, 0.55, 0.01f), glm::vec3(3.5f, 3.5f, 3.5f), 0.0f);
    return SDF::unionOperation(SDF::unionOperation(light, wc), lc);
}