#include <iostream>

#include "types.h"

#include <cmath>
#include <random>
#include <vector>

namespace
{
    static constexpr float Pi = 3.14159265;
    static constexpr float DegToRad = Pi / 180.f;

    static constexpr float RayHitThreshold = 0.001f;

    struct Material;
    struct Sphere
    {
        float3 centerWS;
        float radius;
        const Material* material;
    };

    struct Ray
    {
        float3 origin;
        float3 direction; // Not normalized
    };

    float hit_sphere(const Sphere& sphere, const Ray& ray)
    {
        const float3 originToCenter = ray.origin - sphere.centerWS;
        const float a = dot(ray.direction, ray.direction);
        const float b = 2.f * dot(originToCenter, ray.direction);
        const float c = dot(originToCenter, originToCenter) - sphere.radius * sphere.radius;
        const float discriminant = b * b - 4.f * a * c;

        if (discriminant < 0.f)
            return -1.0;
        else
            return ((-b - sqrt(discriminant)) * 0.5) / a;
    }

    struct Material
    {
        float3 albedo;
        float metalness;
        bool fakeTexture;
    };

    struct RayHit
    {
        float t;
        const Material* mat;

        float3 positionWS;
        float3 normalWS;
    };

    bool isBetterHit(float value, float base)
    {
        return value > RayHitThreshold && (!(base > RayHitThreshold) || value < base);
    }

    struct Scene
    {
        std::vector<const Sphere*> spheres;
    };

    bool traverse_scene(const Scene& s, const Ray& ray, RayHit& hit)
    {
        float bestHit = 0.f;

        for (const Sphere* object : s.spheres)
        {
            const Sphere& sphere = *object;
            const float t = hit_sphere(sphere, ray);

            if (isBetterHit(t, bestHit))
            {
                bestHit = t;
                const float3 rayHitWS = ray.origin + ray.direction * t;
                hit.normalWS = (rayHitWS - sphere.centerWS) / sphere.radius;
                hit.positionWS = rayHitWS;
                hit.mat = sphere.material;
            }
        }

        hit.t = bestHit;
        return bestHit > RayHitThreshold;
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

    float3 shade(const Scene& scene, const Ray& ray, float steps)
    {
        RayHit hitResult;

        // Seed with a real random value, if available
        std::random_device randomDevice;

        // Choose a random mean between 1 and 6
        std::default_random_engine e1(randomDevice());
        std::uniform_real_distribution<float> uniform_dist(-1.f, 1.f);

        if (traverse_scene(scene, ray, hitResult))
        {
            const Material* mat = hitResult.mat;

            if (steps > 0)
            {
                if (mat->metalness == 1.f)
                {
                    const Ray reflectedRay = { hitResult.positionWS, reflect(ray.direction, hitResult.normalWS) };
                    return shade(scene, reflectedRay, steps - 1);
                }
                else
                {
                    float3 albedo = mat->albedo;
                    if (mat->fakeTexture)
                    {
                        // Checkerboard formula
                        const float size = 1.f;
                        const float modx1 = fmodf(fabsf(hitResult.positionWS.x) + size * 0.5f, size * 2.f) - size;
                        const float mody1 = fmodf(fabsf(hitResult.positionWS.z) + size * 0.5f, size * 2.f) - size;

                        if (modx1 * mody1 > 0.f)
                            albedo = float3{0.1f, 0.1f, 0.15f};
                    }

                    float3 randomSample = normalize({uniform_dist(e1), uniform_dist(e1), uniform_dist(e1)});
                    randomSample = sampleDistribution(mat->metalness, hitResult.normalWS, randomSample);
                    const Ray reflectedRay = { hitResult.positionWS, randomSample };

                    const float3 reflectedColor = shade(scene, reflectedRay, steps - 1);

                    // Shade result
                    return albedo * reflectedColor;
                }
            }
            else
                return mat->albedo;
            //return mul(add(mul(hitResult.normalWS, 0.5f), {0.5f,0.5f,0.5f}), hitResult.albedo);
        }

        // No hit -> fake skybox
        const float3 normalizedDirection = normalize(ray.direction);
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

    // Render params
    const int2 imageSize = {800, 450};
    const float fovDegrees = 100.f;
    const int maxSteps = 10;
    const int antiAliasSampleCount = 100;

    // Scene description
    const Material matMirror = {{0.2f, 0.2f, 0.2f}, 1.0f, false};
    const Material mat2 = {{0.2f, 0.2f, 0.2f}, 0.2f, true};
    const Material matDiffuse = {{0.2f, 0.2f, 0.2f}, 0.4f, false};
    const Material mat4 = {{10.0f, 10.0f, 4.0f}, 0.0f, false};
    const Material mat5 = {{10.0f, 1.0f, 1.0f}, 0.0f, false};
    const Material matEmissiveGreen = {{1.0f, 14.0f, 1.0f}, 0.0f, false};

    const Sphere s1 = {{0.f, 0.f, 0.f}, 1.0f, &matMirror};
    const Sphere s2 = {{0.f, -301.f, 0.f}, 300.f, &mat2};
    const Sphere s3 = {{-2.f, 0.5f, -2.f}, 1.5f, &matDiffuse};
    const Sphere s4 = {{2.f, -0.1f, 1.f}, 0.9f, &matDiffuse};
    const Sphere s5 = {{-4.f, 0.f, 0.f}, 1.0f, &mat4};
    const Sphere sphereEmissiveRed = {{4.f, -0.2f, 1.f}, 0.8f, &mat5};
    const Sphere sphereEmissiveGreen = {{-1.3f, -0.5f, 1.5f}, 0.5f, &matEmissiveGreen};
    const Sphere sphereMirror2 = {{-3.f, 1.0f, 4.f}, 2.f, &matMirror};

    const float3 cameraPositionWS = { 3.f, 3.f, 6.f };
    const float3 cameraTargetWS = { 0.f, -1.f, 0.f };
    const float3 cameraUpVector = { 0.f, 1.f, 0.f };
    const float3x3 cameraOrientationWS = lookAt(cameraPositionWS, cameraTargetWS, cameraUpVector);
    Scene scene = { {&s1, &s2, &s3, &s4, &s5, &sphereEmissiveRed, &sphereEmissiveGreen, &sphereMirror2 } };

    float3* imageData = new float3[imageSize.y * imageSize.x]; // Default-init

    const float aspectRatioInv = static_cast<float>(imageSize.y) / static_cast<float>(imageSize.x);
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
            const float2 imageSizeFloat = float2(imageSize);

            // From top left to bottom right
            const float2 rayIndex = float2{static_cast<float>(i), static_cast<float>(j)} + 0.5f;
            float2 rayDirection2D = (rayIndex - imageSizeFloat * 0.5f) / imageSizeFloat;

            rayDirection2D = rayDirection2D * viewportTransform;
            const float3 rayOrigin = { 0.f, 0.f, 0.f};

            const Ray baseRay = { rayOrigin, { rayDirection2D.x, rayDirection2D.y, -1.f} };

            float3 pixelColor = {};
            for (int k = 0; k < antiAliasSampleCount; k++)
            {
                const float2 rayOffset2D = float2{uniform_dist(e1), uniform_dist(e1)} / imageSizeFloat;
                float3 rayOffset = {rayOffset2D.x, rayOffset2D.y, 0.f};

                if (antiAliasSampleCount == 1)
                    rayOffset = float3{0.f, 0.f, 0.f};

                const Ray rayVS = Ray{baseRay.origin, baseRay.direction + rayOffset};
                const Ray rayWS = Ray{rayVS.origin + cameraPositionWS, cameraOrientationWS * rayVS.direction};

                pixelColor = pixelColor + shade(scene, rayWS, maxSteps);
            }

            // Average AA samples
            imageData[j * imageSize.x + i] = pixelColor * (1.f / static_cast<float>(antiAliasSampleCount));;
        }
    }

    writePPMFile(imageData, imageSize);

    return 0;
}
