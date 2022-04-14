//
// Created by Nemo li on 2022/4/13.
//

#include <iostream>
#include "Renderer.h"
#include "SDF.h"
#include "tbb/parallel_for.h"
#include "Scene.h"

#define M_2PI 6.28318530718f
#define EPSILON 1e-6f
#define MAX_DEPTH 6
#define BIAS 1e-4f
//迭代次数，追踪需要次数长
#define MAXITER 32
//光线行进总长，过短效果不明显
#define MAXDIST 5.0f

using namespace tbb;

unsigned char &Renderer::getInitPng(const int width, const int height, const int channel) {
    auto *image = new unsigned char[width * height * channel];
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int index = (i * height + j) * channel;
            //不初始化数组 默认值为0
            image[index] = 255;
            image[index + 1] = 0;
            image[index + 2] = 0;
            image[index + 3] = 255;
        }
    }
    return *image;
}

unsigned char &Renderer::getCircle(const int cx, const int cy, const int radius) {
    auto *image = new unsigned char[m_width * m_height * m_channel];
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            int index = (i * m_height + j) * 4;
            if (SDF::circleSDF(float(j), float(i), 400, 400, 200) <= 0) {
                image[index] = 255;
                image[index + 1] = 0;
                image[index + 2] = 0;
                image[index + 3] = 255;
            } else {
                image[index] = 0;
                image[index + 1] = 0;
                image[index + 2] = 0;
                image[index + 3] = 255;
            }
        }
    }
    return *image;
}

void Renderer::initialize(int width, int height, int channel) {
    m_width = width;
    m_height = height;
    m_channel = channel;
    delete m_image;
    m_samples = 256;
    m_image = new unsigned char[m_width * m_height * m_channel];
}

unsigned char &Renderer::render() {
    int totalPix = m_width * m_height;
    parallel_for(blocked_range<size_t>(0, m_height * m_width, 5000), [&](blocked_range<size_t> &r) {
        for (size_t i = r.begin(); i != r.end(); ++i) {
            size_t col = i % m_width;
            size_t row = i / m_width;
            glm::vec3 color(0.0);
            //sampling and lighting
            color = sample(float(col) / m_width, float(row) / m_height);

            std::cout << "渲染第" << row << "行 第" << col << "列像素点" << "进度为 " << (i * 1.0f) / float(totalPix) << std::endl;
            //保存到png图片
            color.x /= (color.x + 1.0f);
            color.y /= (color.y + 1.0f);
            color.z /= (color.z + 1.0f);
            drawPixel(row, col, color);
        }
    });
    return *m_image;
}

//x 和 y 分别转化为百分比 [0,1]
//每个像素采样
//float Renderer::sample(float x, float y) {
//    float sum = 0.0f;
//    //对每个像素采样 samples次 360度的光线
//    for (int i = 0; i < m_samples; ++i) {
//        // randome sampling.
//        // float angle = M_2PI * rand() / RAND_MAX;
//        // stratified samping.
//        // float angle = M_2PI * i / m_samples;
//        // Jittered Sampling
//        float angle = M_2PI * (i + static_cast<float>(rand()) / RAND_MAX) / m_samples;
//        sum += trace(x, y, cosf(angle), sinf(angle));
//    }
//    return sum / m_samples;
//}

glm::vec3 Renderer::sample(float x, float y) {
    //辐射度量 光强
    glm::vec3 radiance = glm::vec3(0.0f);
    //对每个像素采样 samples次 360度的光线
    for (int i = 0; i < m_samples; ++i) {
        float angle = M_2PI * (i + float(rand()) / RAND_MAX) / float(m_samples);

        radiance += deNan(trace(x, y, cosf(angle), sinf(angle), 0));
    }
    radiance.x /= m_samples;
    radiance.y /= m_samples;
    radiance.z /= m_samples;
    return radiance;
}

void Renderer::drawPixel(unsigned int row, unsigned int col, const glm::vec3 &color) {
    if (row < 0 || row >= m_height || col < 0 || col >= m_width)
        return;
    unsigned int index = (row * m_width + col) * 4;
    m_image[index + 0] = char(std::min(color.x, 1.0f) * 255.0f);
    m_image[index + 1] = char(std::min(color.y, 1.0f) * 255.0f);
    m_image[index + 2] = char(std::min(color.z, 1.0f) * 255.0f);
    m_image[index + 3] = 255;
}

//光线和物体相交才有光强，不相交没有光强
float Renderer::trace(float ox, float oy, float dx, float dy) {
    float step = 0.0f;
    int maxIter = 10;
    float maxDistance = 2.0f;
    float epsilon = 1e-6f; //0.000001
    for (int i = 0; i < maxIter && step < maxDistance; ++i) {
        //dx 和 dy 表示光线行进方向
//        float sdf = SDF::segmentSDF(ox + dx * step, oy + dy * step, 0.25f, 0.5f, 0.5f, 0.5f);
        float sdf = SDF::triangleSDF(ox + dx * step, oy + dy * step, 0.5f, 0.2f, 0.8f, 0.8f, 0.3f, 0.6f);
//        const Result &result = Scene::moonEmissiveScene(float(ox + dx * step), float(oy + dy * step));
//        float sdf = result.sdf;
        // reach the edge(==0) or inside(<0)
        if (sdf < epsilon)
//            return result.emissive.x;
            return 2.0f;
        step += sdf;
    }
    return 0.0f;
}

glm::vec3 Renderer::trace(float ox, float oy, float dx, float dy, int depth) {
    float step = 1e-3f;
    float sign = scene(ox, oy).sdf > 0.0f ? 1.0f : -1.0f;
    for (int i = 0; i < MAXITER && step < MAXDIST; ++i) {
        float x = ox + dx * step;
        float y = oy + dy * step;
        Result ret = scene(x, y);
        // reach the egde(==0) or inside(<0)
        if (ret.sdf * sign < EPSILON) {
            glm::vec3 sum = ret.emissive;
            if (depth < MAX_DEPTH && (ret.reflectivity > 0.0f || ret.eta > 0.0f)) {
                // get normal and reflect.
                float nx = 0, ny = 0, rx = 0, ry = 0;
                float reflectivity = ret.reflectivity;
                gradient(x, y, nx, ny);
                // inside, need to reverse normal.
                nx *= sign;
                ny *= sign;
                // refraction.
                if (ret.eta > 0.0f) {
                    if (refract(dx, dy, nx, ny, sign < 0.0f ? ret.eta : 1.0f / ret.eta, rx, ry)) {
                        float cosi = -(dx * nx + dy * ny);
                        float cost = -(rx * nx + ry * ny);
                        reflectivity = sign < 0.0f ? fresnel(cosi, cost, ret.eta, 1.0f)
                                                   : fresnel(cosi, cost, 1.0f, ret.eta);
                        sum += (1.0f - reflectivity) * trace(x - nx * BIAS, y - ny * BIAS, rx, ry, depth + 1);
                    } else
                        reflectivity = 1.0f;// full reflect.
                }
                // reflection.
                if (reflectivity > 0.0f) {
                    reflect(dx, dy, nx, ny, rx, ry);
                    sum += reflectivity * trace(x + nx * BIAS, y + ny * BIAS, rx, ry, depth + 1);
                }
            }
            return sum * beerLambert(ret.absorption, step);
        }
        step += ret.sdf * sign;
    }
    return glm::vec3(0.0f);
}

Result Renderer::scene(float x, float y) {
    // scene 1.
    //Result ret = Scene::oneEmissiveSphereScene(ox + dx * step, oy + dy * step);
    // scene 2.
    //Result ret = Scene::threeEmissiveSphereScene(x, y);
    // scene 3.
    //Result ret = Scene::moonEmissiveScene(ox + dx * step, oy + dy * step);
    // scene 4.
    //Result ret = Scene::planeEmissiveScene(ox + dx * step, oy + dy * step);
    // scene 5.
    //Result ret = Scene::capsuleEmissiveScene(ox + dx * step, oy + dy * step);
    // scene 6.
    //Result ret = Scene::boxEmissiveScene(ox + dx * step, oy + dy * step);
    // scene 7.
    //Result ret = Scene::triangleEmissiveScene(x, y);
    // scene 8.
    //Result ret = Scene::reflectEmissiveScene(x, y);
    // scene 9.
    //Result ret = Scene::causticEmissiveScene(x, y);
    // scene 10.
    //Result ret = Scene::rectRefractEmissiveScene(x, y);
    // scene 11.
    //Result ret = Scene::refractEmissiveScene(x, y);
    // scene 12.
    //Result ret = Scene::beerLambertScene(x, y);
    // scene 13.
    //Result ret = Scene::heartScene(x, y);
    // scene 14.
    //Result ret = Scene::finalScene(x, y);
    // scene 15.
    //Result ret = Scene::nameScene(x, y);
    // scene 16.
    //Result ret = Scene::sampleReflectScene(x, y);
    Result ret = Scene::WLScene(x, y);
    return ret;
}

glm::vec3 Renderer::beerLambert(glm::vec3 a, float d) {
    return glm::vec3(expf(-a.x * d), expf(-a.y * d), expf(-a.z * d));
}

void Renderer::gradient(float x, float y, float &nx, float &ny) {
    nx = (scene(x + EPSILON, y).sdf - scene(x - EPSILON, y).sdf) * (0.5f / EPSILON);
    ny = (scene(x, y + EPSILON).sdf - scene(x, y - EPSILON).sdf) * (0.5f / EPSILON);
}

float Renderer::fresnel(float cosi, float cost, float etai, float etat) {
    float rs = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
    float rp = (etai * cosi - etat * cost) / (etai * cosi + etat * cost);
    return (rs * rs + rp * rp) * 0.5f;
}

void Renderer::reflect(float ix, float iy, float nx, float ny, float &rx, float &ry) {
    float idotn_2 = (ix * nx + iy * ny) * 2.0f;
    rx = ix - idotn_2 * nx;
    ry = iy - idotn_2 * ny;
}

int Renderer::refract(float ix, float iy, float nx, float ny, float eta, float &rx, float &ry) {
    float idotn = ix * nx + iy * ny;
    float k = 1.0f - eta * eta * (1.0f - idotn * idotn);
    if (k < 0.0f)
        return 0;
    float a = eta * idotn + sqrtf(k);
    rx = eta * ix - a * nx;
    ry = eta * iy - a * ny;
    return 1;
}