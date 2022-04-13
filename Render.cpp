//
// Created by Nemo li on 2022/4/13.
//

#include "Render.h"
#include "SDF.h"

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
}
