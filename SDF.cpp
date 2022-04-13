//
// Created by Nemo li on 2022/4/13.
//

#include <math.h>
#include "SDF.h"

float SDF::circleSDF(float x, float y, float cx, float cy, float radius) {
    //圆的sdf为 像素点到圆心的距离减去圆的半径 (cx, cy) 圆心
    float ux = x - cx, uy = y - cy;
    return sqrtf(ux * ux + uy * uy) - radius;
}
