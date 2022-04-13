//
// Created by Nemo li on 2022/4/13.
//

#include "Renderer.h"
#include "SDF.h"
#include "tbb/parallel_for.h"
#include "Scene.h"

#define M_2PI 6.28318530718f
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
    m_samples = 64;
    m_image = new unsigned char[m_width * m_height * m_channel];
}

unsigned char &Renderer::render() {
    parallel_for(blocked_range<size_t>(0, m_height * m_width, 5000), [&](blocked_range<size_t> &r) {
        for (size_t i = r.begin(); i != r.end(); ++i) {
            size_t col = i % m_width;
            size_t row = i / m_width;
            glm::vec3 color(0.0);
            //sampling and lighting
            color.x = color.y = color.z =
                    sample(float(col) / m_width, float(row) / m_height);
            //保存到png图片
            drawPixel(row, col, color);
        }
    });
    return *m_image;
}

//x 和 y 分别转化为百分比 [0,1]
//每个像素采样
float Renderer::sample(float x, float y) {
    float sum = 0.0f;
    //对每个像素采样 samples次 360度的光线
    for (int i = 0; i < m_samples; ++i) {
        // randome sampling.
        // float angle = M_2PI * rand() / RAND_MAX;
        // stratified samping.
        // float angle = M_2PI * i / m_samples;
        // Jittered Sampling
        float angle = M_2PI * (i + static_cast<float>(rand()) / RAND_MAX) / m_samples;
        sum += trace(x, y, cosf(angle), sinf(angle));
    }
    return sum / m_samples;
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
        //float sdf = SDF::circleSDF(ox + dx * step, oy + dy * step, 0.5f, 0.5f, 0.1f);
        const Result &result = Scene::threeEmissiveSphereScene(float(ox + dx * step), float(oy + dy * step));
        float sdf = result.sdf;
        // reach the edge(==0) or inside(<0)
        if (sdf < epsilon)
            return result.emissive.x;
        step += sdf;
    }
    return 0.0f;
}