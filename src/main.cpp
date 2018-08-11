#include <iostream>

#include "types.h"
#include "ray.h"

#include <cmath>

namespace
{
    static constexpr float Pi = 3.14159265;
    static constexpr float DegToRad = Pi / 180.f;

    bool hit_sphere(const float3& center, float radius, const ray& r)
    {
        const float3 originToCenter = sub(r.origin, center);
        const float a = dot(r.direction, r.direction);
        const float b = 2.f * dot(originToCenter, r.direction);
        const float c = dot(originToCenter, originToCenter) - radius * radius;
        const float discriminant = b * b - 4.f * a * c;

        return discriminant > 0.f;
    }

    float3 color(const ray& r)
    {
        if (hit_sphere(float3{0.f, 0.f, -1.f}, 0.5, r))
            return float3{1.f, 0.f, 0.f};
        const float3 normalizedDirection = normalize(r.direction);
        const float t = 0.5f * (normalizedDirection.y + 1.f);
        return add(mul(float3{1.f, 1.f, 1.f}, 1.f - t), mul(float3{0.5f, 0.7f, 1.f}, t));
    }
}

int main(int argc, char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    const int2 imageSize = {200, 100};

    const float aspectRatio = static_cast<float>(imageSize.x) / static_cast<float>(imageSize.y);
    const float aspectRatioInv = 1.f / aspectRatio;
    const float fovDegrees = 120;
    const float tanHFov = tan((fovDegrees * 0.5) * DegToRad);
    const float2 viewportTransform = { tanHFov, -tanHFov * aspectRatioInv };

    std::cout << "P3" << std::endl << imageSize.x << ' ' << imageSize.y << std::endl << "255" << std::endl;

    for (int j = 0; j < imageSize.y; j++)
    {
        for (int i = 0; i < imageSize.x; i++)
        {
            const float2 imageSizeFloat = {
                static_cast<float>(imageSize.x),
                static_cast<float>(imageSize.y)
            };

            // From top left to bottom right
            const float2 rayIndex = add(float2{ static_cast<float>(i), static_cast<float>(j) }, float2{0.5f, 0.5f});
            float2 rayDirection2D = div(sub(rayIndex, mul(imageSizeFloat, 0.5f)), imageSizeFloat);

            rayDirection2D = mul(rayDirection2D, viewportTransform);
            const float3 rayOrigin = { 0.f, 0.f, 0.f};

            const ray rayCast = { rayOrigin, { rayDirection2D.x, rayDirection2D.y, -1.f} };

            const float3 pixelColor = color(rayCast);

            const int3 pixelColorQuantized = {
                static_cast<int>(pixelColor.x * 255.f),
                static_cast<int>(pixelColor.y * 255.f),
                static_cast<int>(pixelColor.z * 255.f)
            };
            std::cout << pixelColorQuantized.x << ' ' << pixelColorQuantized.y << ' ' << pixelColorQuantized.z << std::endl;
        }
    }

    return 0;
}
