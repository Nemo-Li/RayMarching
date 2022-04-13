//
// Created by Nemo li on 2022/4/13.
//

#ifndef INC_2DRAYMARCHINGRENDERER_RENDER_H
#define INC_2DRAYMARCHINGRENDERER_RENDER_H


class Render {
private:
    int m_width, m_height, m_channel;
    unsigned char *m_image;

public:
    void initialize(int width, int height, int channel);

    unsigned char &getInitPng(const int width, const int height, const int channel);

    unsigned char &getCircle(const int cx, const int cy, const int radius);
};


#endif //INC_2DRAYMARCHINGRENDERER_RENDER_H
