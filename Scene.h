//
// Created by Nemo li on 2022/4/13.
//

#ifndef INC_2DRAYMARCHINGRENDERER_SCENE_H
#define INC_2DRAYMARCHINGRENDERER_SCENE_H

#include "SDF.h"

class Scene {
public:
    static Result threeEmissiveSphereScene(float x, float y);

    static Result moonEmissiveScene(float x, float y);

    static Result sampleReflectScene(float x, float y);

    static Result beerLambertScene(float x, float y);

    static Result rectRefractEmissiveScene(float x, float y);

    static Result WScene(float x, float y);

    static Result WLScene(float x, float y);
};


#endif //INC_2DRAYMARCHINGRENDERER_SCENE_H
