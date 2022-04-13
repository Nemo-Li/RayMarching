//
// Created by Nemo li on 2022/4/13.
//

#ifndef INC_2DRAYMARCHINGRENDERER_RENDER_H
#define INC_2DRAYMARCHINGRENDERER_RENDER_H


#include <libunwind.h>
#include "glm/glm.hpp"

class Render {
private:
    int m_width, m_height, m_channel;
    unsigned char *m_image;

public:
    void initialize(int width, int height, int channel);

    static unsigned char &getInitPng(const int width, const int height, const int channel);

    unsigned char &getCircle(const int cx, const int cy, const int radius);

    unsigned char &render();

    void drawPixel(unsigned int row, unsigned int col, const glm::vec3 &color);
};


#endif //INC_2DRAYMARCHINGRENDERER_RENDER_H
