//
// Created by Nemo li on 2022/4/13.
//

#include <math.h>
#include "SDF.h"
#define M_2PI 6.28318530718f

float SDF::circleSDF(float x, float y, float cx, float cy, float radius) {
    //圆的sdf为 像素点到圆心的距离减去圆的半径 (cx, cy) 圆心
    float ux = x - cx, uy = y - cy;
    return sqrtf(ux * ux + uy * uy) - radius;
}

float SDF::planeSDF(float x, float y, float px, float py, float nx, float ny)
{
    return (x - px)*nx + (y - py)*ny;
}

//线段sdf
float SDF::segmentSDF(float x, float y, float ax, float ay, float bx, float by)
{
    float vx = x - ax, vy = y - ay;
    float ux = bx - ax, uy = by - ay;
    float t = fmaxf(fminf((vx * ux + vy * uy) / (ux * ux + uy * uy), 1.0f), 0.0f);
    float dx = vx - ux * t, dy = vy - uy * t;
    return sqrtf(dx * dx + dy * dy);
}

float SDF::capsuleSDF(float x, float y, float ax, float ay, float bx, float by, float radius)
{
    return segmentSDF(x, y, ax, ay, bx, by) - radius;
}

//多边形
float SDF::ngonSDF(float x, float y, float cx, float cy, float r, float n) {
    float ux = x - cx, uy = y - cy, a = M_2PI / n;
    float t = fmodf(atan2f(uy, ux) + M_2PI, a), s = sqrtf(ux * ux + uy * uy);
    return planeSDF(s * cosf(t), s * sinf(t), r, 0.0f, cosf(a * 0.5f), sinf(a * 0.5f));
}

//矩形sdf函数 定向包围盒 OBB 轴向包围盒 AABB
//计算公式参考：https://zhuanlan.zhihu.com/p/420700051
//glsl
// float myBoxSDF(in vec2 p, in vec2 a)
//{
//    vec2 q = abs(p)-a;
//    return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0);
//}
//从任意旋转位置，回到世界坐标系原点，需要矩阵旋转以及平移操作
float SDF::boxSDF(float x, float y, float cx, float cy, float theta, float sx, float sy)
{
	float cosTheta = cosf(theta), sinTheta = sinf(theta);
	//dx dy 表示q向量 右上顶点s到任意点p的向量
	float dx = fabs((x - cx) * cosTheta + (y - cy)*sinTheta) - sx;
	float dy = fabs((y - cy) * cosTheta - (x - cx)*sinTheta) - sy;
	float ax = fmaxf(dx, 0.0f), ay = fmaxf(dy, 0.0f);
	//第一项 第4象限 第二项 第1，2，3象限
	return fminf(fmaxf(dx, dy), 0.0f) + sqrtf(ax * ax + ay * ay);
}

float SDF::triangleSDF(float x, float y, float ax, float ay, float bx, float by, float cx, float cy)
{
    //距离3条线段的最短距离
    float d = fminf(fminf(
            segmentSDF(x, y, ax, ay, bx, by),
            segmentSDF(x, y, bx, by, cx, cy)),
                    segmentSDF(x, y, cx, cy, ax, ay));
    return
            (bx - ax) * (y - ay) > (by - ay) * (x - ax) &&
            (cx - bx) * (y - by) > (cy - by) * (x - bx) &&
            (ax - cx) * (y - cy) > (ay - cy) * (x - cx) ? -d : d;
}

float SDF::WcharSDF(float x, float y, float cx, float cy, float radius)
{
    glm::vec2 point[5];
    point[0] = glm::vec2(0.0, -0.06) + glm::vec2(cx, cy);
    point[1] = glm::vec2(-0.03, +0.06) + glm::vec2(cx, cy);
    point[2] = glm::vec2(+0.03, +0.06) + glm::vec2(cx, cy);
    point[3] = glm::vec2(-0.06, -0.06) + glm::vec2(cx, cy);
    point[4] = glm::vec2(+0.06, -0.06) + glm::vec2(cx, cy);
    float c1 = capsuleSDF(x, y, point[0].x, point[0].y, point[1].x, point[1].y, radius);
    float c2 = capsuleSDF(x, y, point[1].x, point[1].y, point[3].x, point[3].y, radius);
    float c3 = capsuleSDF(x, y, point[0].x, point[0].y, point[2].x, point[2].y, radius);
    float c4 = capsuleSDF(x, y, point[2].x, point[2].y, point[4].x, point[4].y, radius);
    return fminf(fminf(c1, c2), fminf(c3, c4));
}

float SDF::LcharSDF(float x, float y, float cx, float cy, float radius)
{
    glm::vec2 point[4];
    point[1] = glm::vec2(-0.05, -0.06) + glm::vec2(cx, cy);
    point[2] = glm::vec2(-0.05, +0.06) + glm::vec2(cx, cy);
    point[3] = glm::vec2(+0.05, +0.06) + glm::vec2(cx, cy);
    float c2 = capsuleSDF(x, y, point[1].x, point[1].y, point[2].x, point[2].y, radius);
    float c3 = capsuleSDF(x, y, point[2].x, point[2].y, point[3].x, point[3].y, radius);
    return fminf(c2, c3);
}

Result SDF::unionOperation(Result a, Result b)
{
    return a.sdf < b.sdf ? a : b;
}

Result SDF::intersectOperation(Result a, Result b)
{
    Result ret = a.sdf < b.sdf ? b : a;
    return ret;
}

Result SDF::subtractOperation(Result a, Result b)
{
    Result ret = a;
    ret.sdf = (a.sdf > -b.sdf) ? a.sdf : -b.sdf;
    return ret;
}