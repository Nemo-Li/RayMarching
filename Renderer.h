//
// Created by Nemo li on 2022/4/13.
//

#ifndef INC_2DRAYMARCHINGRENDERER_RENDERER_H
#define INC_2DRAYMARCHINGRENDERER_RENDERER_H

#include <libunwind.h>
#include "glm/glm.hpp"
#include "SDF.h"

class Renderer {
private:
    int m_samples;
    int m_width, m_height, m_channel;
    unsigned char *m_image;

public:
    void initialize(int width, int height, int channel);

    static unsigned char &getInitPng(const int width, const int height, const int channel);

    unsigned char &getCircle(const int cx, const int cy, const int radius);

    unsigned char &render();

    void drawPixel(unsigned int row, unsigned int col, const glm::vec3 &color);

    glm::vec3 sample(float x, float y);

    static float trace(float ox, float oy, float dx, float dy);

    glm::vec3 trace(float ox, float oy, float dx, float dy, int depth);

    void gradient(float x, float y, float &nx, float &ny);

    Result scene(float x, float y);

    glm::vec3 deNan(const glm::vec3 &target) {
        glm::vec3 temp = target;
        if (!(temp[0] == temp[0]))temp[0] = 0;
        if (!(temp[1] == temp[1]))temp[1] = 0;
        if (!(temp[2] == temp[2]))temp[2] = 0;
        return temp;
    }

    static void reflect(float ix, float iy, float nx, float ny, float &rx, float &ry);

    static int refract(float ix, float iy, float nx, float ny, float eta, float &rx, float &ry);

    static float fresnel(float cosi, float cost, float etai, float etat);

    static glm::vec3 beerLambert(glm::vec3 a, float d);
};


#endif //INC_2DRAYMARCHINGRENDERER_RENDERER_H
