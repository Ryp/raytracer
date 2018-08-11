#include <iostream>

#include "types.h"

namespace
{
    void printDummyPPMFile()
    {
        const int2 imageSize = {200, 100};

        std::cout << "P3" << std::endl << imageSize.x << ' ' << imageSize.y << std::endl << "255" << std::endl;

        for (int j = 0; j < imageSize.y; j++)
        {
            for (int i = 0; i < imageSize.x; i++)
            {
                const float3 pixelColor = {
                    static_cast<float>(i) / static_cast<float>(imageSize.x - 1),
                    static_cast<float>(j) / static_cast<float>(imageSize.y - 1),
                    0.2f
                };
                const int3 pixelColorQuantized = {
                    static_cast<int>(pixelColor.x * 255.f),
                    static_cast<int>(pixelColor.y * 255.f),
                    static_cast<int>(pixelColor.z * 255.f)
                };
                std::cout << pixelColorQuantized.x << ' ' << pixelColorQuantized.y << ' ' << pixelColorQuantized.z << std::endl;
            }
        }
    }
}

int main(int argc, char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    printDummyPPMFile();

    return 0;
}
