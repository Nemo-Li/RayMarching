//
// Created by Nemo li on 2022/4/13.
//

#include "Render.h"
#include "SDF.h"
#include "tbb/parallel_for.h"

using namespace tbb;

unsigned char &Render::getInitPng(const int width, const int height, const int channel) {
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

unsigned char &Render::getCircle(const int cx, const int cy, const int radius) {
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

void Render::initialize(int width, int height, int channel) {
    m_width = width;
    m_height = height;
    m_channel = channel;
    delete m_image;
    m_image = new unsigned char[m_width * m_height * m_channel];
}

unsigned char &Render::render() {
    parallel_for(blocked_range<size_t>(0, m_height * m_width, 5000), [&](blocked_range <size_t> &r) {
        for (size_t i = r.begin(); i != r.end(); ++i) {
            size_t col = i % m_width;
            size_t row = i / m_width;
            glm::vec3 color(0.0);
            drawPixel(row, col, color);
        }
    });
    return *m_image;
}

void Render::drawPixel(unsigned int row, unsigned int col, const glm::vec3 &color) {
    if (row < 0 || row >= m_height || col < 0 || col >= m_width)
        return;
    unsigned int index = (row * m_width + col) * 4;
    m_image[index + 0] = char(std::min(color.x, 1.0f) * 255.0f);
    m_image[index + 1] = char(std::min(color.y, 1.0f) * 255.0f);
    m_image[index + 2] = char(std::min(color.z, 1.0f) * 255.0f);
    m_image[index + 3] = 255;
}
