#include "texture.h"
#include <algorithm>
#include "stb_image.h"

ImageTexture::ImageTexture(const std::string &filename) {
    int n = 0;
    data = stbi_load(filename.c_str(), &width, &height, &n, 3);
    bytes_per_scanline = width * 3;
}

ImageTexture::~ImageTexture() {
    if (data) {
        stbi_image_free(data);
    }
}

Color ImageTexture::value(FloatType u, FloatType v, const Point3 &p) const {
    if (data == nullptr) {
        return Color(0, 1, 1);
    }

    u = std::clamp(u, static_cast<FloatType>(0.0), static_cast<FloatType>(1.0));
    v = 1.0 - std::clamp(v, static_cast<FloatType>(0.0), static_cast<FloatType>(1.0));

    int i = static_cast<int>(u * width);
    int j = static_cast<int>(v * height);
    if (i >= width) i = width - 1;
    if (j >= height) j = height - 1;

    const unsigned char *pixel = data + j * bytes_per_scanline + i * 3;
    FloatType scale = 1.0 / 255.0;
    return Color(scale * pixel[0], scale * pixel[1], scale * pixel[2]);
}
