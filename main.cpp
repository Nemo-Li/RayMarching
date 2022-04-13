#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"
#include "Render.h"

int main() {
    const int width = 800, height = 800, channel = 4;
    Render render;
    unsigned char &image = render.getInitPng(width, height, channel);
    stbi_write_png("./result.png", width, height, channel, &image, width * 4);
    return 0;
}

