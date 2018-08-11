#include "types.h"

#include <cmath>

float3 add(const float3& v1, const float3& v2)
{
    return {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z
    };
}

float3 sub(const float3& v1, const float3& v2)
{
    return {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z
    };
}

float3 mul(const float3& vec, float scalar)
{
    return {
        vec.x * scalar,
        vec.y * scalar,
        vec.z * scalar
    };
}

float3 mul(const float3& v1, const float3& v2)
{
    return {
        v1.x * v2.x,
        v1.y * v2.y,
        v1.z * v2.z
    };
}

float3 div(const float3& v1, const float3& v2)
{
    return {
        v1.x / v2.x,
        v1.y / v2.y,
        v1.z / v2.z
    };
}

float lengthSqr(const float3& vec)
{
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

float3 normalize(const float3& vec)
{
    const float length = sqrt(lengthSqr(vec));
    const float lengthInv = 1.f / length;

    return mul(vec, lengthInv);
}

float dot(const float3& v1, const float3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float3 cross(const float3& v1, const float3& v2)
{
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
    };
}

float3 saturate(const float3& vec)
{
    return {
        fmax(fmin(vec.x, 1.f), 0.f),
        fmax(fmin(vec.y, 1.f), 0.f),
        fmax(fmin(vec.z, 1.f), 0.f)
    };
}
