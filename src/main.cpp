#include <iostream>

#include "types.h"
#include "ray.h"

#include <cmath>
#include <random>

namespace
{
    static constexpr float Pi = 3.14159265;
    static constexpr float DegToRad = Pi / 180.f;

    struct sphere
    {
        float3 center;
        float radius;
    };

    float hit_sphere(const sphere& s, const ray& r)
    {
        const float3 originToCenter = r.origin - s.center;
        const float a = dot(r.direction, r.direction);
        const float b = 2.f * dot(originToCenter, r.direction);
        const float c = dot(originToCenter, originToCenter) - s.radius * s.radius;
        const float discriminant = b * b - 4.f * a * c;

        if (discriminant < 0.f)
            return -1.0;
        else
            return ((-b - sqrt(discriminant)) * 0.5) / a;
    }

    struct rayHit
    {
        float t;
        float3 normalWS;
        float3 albedo;
        float metalness;
    };

    bool isBetterHit(float value, float base)
    {
        return value > 0.f && (base <= 0.f || value < base);
    }

    bool traverse_scene(const ray& r, rayHit& hit)
    {
        const sphere s1 = {{0.f, 0.f, -3.f}, 1.0f};
        const sphere s2 = {{0.f, -501.f, -3.f}, 500.f};

        float bestHit = 0.f;

        const float t1 = hit_sphere(s1, r);
        if (isBetterHit(t1, bestHit))
        {
            bestHit = t1;
            const float3 rayHitWS = r.origin + r.direction * t1;
            hit.normalWS = (rayHitWS - s1.center) / s1.radius;
            hit.albedo = {1.f, 1.f, 1.f};
            hit.metalness = 0.99f;
        }

        const float t2 = hit_sphere(s2, r);
        if (isBetterHit(t2, bestHit))
        {
            bestHit = t2;
            const float3 rayHitWS = r.origin + r.direction * t2;
            hit.normalWS = (rayHitWS - s2.center) / s2.radius;
            hit.albedo = {0.1f, 0.3f, 0.3f};
            hit.metalness = 0.2f;
        }

        hit.t = bestHit;

        return bestHit > 0.f;
    }

    float sign(float value)
    {
        // Could bitmask here
        return (value >= 0.f) ? 1.f : -1.f;
    }

    float3 sampleDistribution(float metalness, float3 normal, float3 sample)
    {
        static_cast<void>(metalness);

        const float3 sampleOriented = sample * sign(dot(normal, sample));
        return sampleOriented;
    }

    float3 shade(const ray& r, float steps)
    {
        rayHit hitResult;

        // Seed with a real random value, if available
        std::random_device randomDevice;

        // Choose a random mean between 1 and 6
        std::default_random_engine e1(randomDevice());
        std::uniform_real_distribution<float> uniform_dist(-1.f, 1.f);

        if (traverse_scene(r, hitResult))
        {
            const float3 albedo = hitResult.albedo;

            if (steps > 0)
            {
                const float sampleCount = 8 * steps;

                float3 reflectedColor = {};
                for (int k = 0; k < sampleCount; k++)
                {
                    const float3 rayHitWS = r.origin + r.direction * hitResult.t;
                    float3 randomSample = normalize(
                        {
                            uniform_dist(e1),
                            uniform_dist(e1),
                            uniform_dist(e1)
                        }
                    );
                    randomSample = sampleDistribution(hitResult.metalness, hitResult.normalWS, randomSample);
                    const ray reflectedRay = { rayHitWS, randomSample };

                    reflectedColor = reflectedColor + shade(reflectedRay, steps - 1);
                }
                reflectedColor = reflectedColor * (1.f / static_cast<float>(sampleCount));

                // Shade result
                return albedo * reflectedColor;
            }
            else
                return albedo;
            //return mul(add(mul(hitResult.normalWS, 0.5f), {0.5f,0.5f,0.5f}), hitResult.albedo);
        }

        // No hit -> fake skybox
        const float3 normalizedDirection = normalize(r.direction);
        const float t = 0.5f * (normalizedDirection.y + 1.f);
        return float3{1.f, 1.f, 1.f} * (1.f - t) + float3{0.5f, 0.7f, 1.f} * t;
    }

    // Takes linear input data
    void writePPMFile(const float3* imageData, const int2& imageSize)
    {
        std::cout << "P3" << std::endl << imageSize.x << ' ' << imageSize.y << std::endl << "255" << std::endl;

        for (int j = 0; j < imageSize.y; j++)
        {
            for (int i = 0; i < imageSize.x; i++)
            {
                const int pixelIndex = imageSize.x * j + i;
                const float3 pixelColor = imageData[pixelIndex];

                // PPM gamma correction
                const float ppmGamma = 2.2f;
                const float ppmGammaInv = 1.f / ppmGamma;

                const int3 pixelColorQuantized = int3(pow(pixelColor, ppmGammaInv) * 255.f);
                std::cout << pixelColorQuantized.x << ' ' << pixelColorQuantized.y << ' ' << pixelColorQuantized.z << std::endl;
            }
        }
    }
}

int main(int argc, char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    const int2 imageSize = {200, 150};
    const float fovDegrees = 130.f;
    const int maxSteps = 2;
    const int antiAliasSampleCount = 4;

    float3 imageData[imageSize.y][imageSize.x]; // Default-init

    const float aspectRatio = static_cast<float>(imageSize.x) / static_cast<float>(imageSize.y);
    const float aspectRatioInv = 1.f / aspectRatio;
    const float tanHFov = tan((fovDegrees * 0.5) * DegToRad);
    const float2 viewportTransform = { tanHFov, -tanHFov * aspectRatioInv };

    // Seed with a real random value, if available
    std::random_device randomDevice;

    // Choose a random mean between 1 and 6
    std::default_random_engine e1(randomDevice());
    std::uniform_real_distribution<float> uniform_dist(-0.5f, 0.5);

    for (int j = 0; j < imageSize.y; j++)
    {
        for (int i = 0; i < imageSize.x; i++)
        {
            const float2 imageSizeFloat = {
                static_cast<float>(imageSize.x),
                static_cast<float>(imageSize.y)
            };

            // From top left to bottom right
            const float2 rayIndex = float2{static_cast<float>(i), static_cast<float>(j)} + 0.5f;
            float2 rayDirection2D = (rayIndex - imageSizeFloat * 0.5f) / imageSizeFloat;

            rayDirection2D = rayDirection2D * viewportTransform;
            const float3 rayOrigin = { 0.f, 0.f, 0.f};

            const ray baseRay = { rayOrigin, { rayDirection2D.x, rayDirection2D.y, -1.f} };

            float3 pixelColor = {};
            for (int k = 0; k < antiAliasSampleCount; k++)
            {
                const float3 rayOffset = { uniform_dist(e1) / imageSizeFloat.x, uniform_dist(e1) / imageSizeFloat.y, 0.f};
                const ray r = ray{baseRay.origin, baseRay.direction + rayOffset};
                pixelColor = pixelColor + shade(r, maxSteps);
            }

            // Average AA samples
            imageData[j][i] = pixelColor * (1.f / static_cast<float>(antiAliasSampleCount));;
        }
    }

    writePPMFile(&imageData[0][0], imageSize);

    return 0;
}
