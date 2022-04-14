//
// Created by Nemo li on 2022/4/13.
//

#ifndef INC_2DRAYMARCHINGRENDERER_SDF_H
#define INC_2DRAYMARCHINGRENDERER_SDF_H

#include "glm/glm.hpp"

struct Result {
    //距离
    float sdf;
    //光强
    glm::vec3 emissive;
    //反射
    float reflectivity;
    float eta;
    glm::vec3 absorption;

    Result() {}

    Result(float s, glm::vec3 e) {
        sdf = s;
        emissive = e;
    }

    Result(float s, glm::vec3 e, float r, float et = 0.0f, glm::vec3 absor = glm::vec3(0.0f)) {
        sdf = s;
        emissive = e;
        reflectivity = r;
        eta = et;
        absorption = absor;
    }
};


class SDF {
public:
    // several shape sdf.
    static float circleSDF(float x, float y, float cx, float cy, float radius);

    static float segmentSDF(float x, float y, float ax, float ay, float bx, float by);


    // sdf operation.
    static Result unionOperation(Result a, Result b);

    static Result intersectOperation(Result a, Result b);

    static Result subtractOperation(Result a, Result b);

    static float capsuleSDF(float x, float y, float ax, float ay, float bx, float by, float radius);

    static float boxSDF(float x, float y, float cx, float cy, float theta, float sx, float sy);

    static float triangleSDF(float x, float y, float ax, float ay, float bx, float by, float cx, float cy);
};


#endif //INC_2DRAYMARCHINGRENDERER_SDF_H
