#include <vector>
#include "spdlog/spdlog.h"
#include "stb_image_write.h"

int main()
{
    spdlog::info("Starting image generation...");

    constexpr int image_width = 256;
    constexpr int image_height = 256;
    constexpr int channels = 3;

    std::vector<unsigned char> pixels(image_width * image_height * channels);

    for (int j = 0; j < image_height; ++j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0.0;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            pixels[channels * (j * image_width + i) + 0] = ir;
            pixels[channels * (j * image_width + i) + 1] = ig;
            pixels[channels * (j * image_width + i) + 2] = ib;
        }
    }

    const char *filename = "output.png";
    int stride_in_bytes = image_width * channels;

    if (stbi_write_png(filename, image_width, image_height, channels, pixels.data(), stride_in_bytes))
    {
        spdlog::info("Successfully saved image to {}", filename);
    }
    else
    {
        spdlog::error("Failed to save image to {}", filename);
        return 1;
    }

    return 0;
}