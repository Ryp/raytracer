#include "types.h"

#include <cmath>

float2 add(const float2& v1, const float2& v2)
{
    return {
        v1.x + v2.x,
        v1.y + v2.y
    };
}

float2 sub(const float2& v1, const float2& v2)
{
    return {
        v1.x - v2.x,
        v1.y - v2.y
    };
}

float2 mul(const float2& vec, float scalar)
{
    return {
        vec.x * scalar,
        vec.y * scalar
    };
}

float2 mul(const float2& v1, const float2& v2)
{
    return {
        v1.x * v2.x,
        v1.y * v2.y
    };
}

float2 div(const float2& v1, const float2& v2)
{
    return {
        v1.x / v2.x,
        v1.y / v2.y
    };
}

float lengthSqr(const float2& vec)
{
    return vec.x * vec.x + vec.y * vec.y;
}

float2 normalize(const float2& vec)
{
    const float length = sqrt(lengthSqr(vec));
    const float lengthInv = 1.f / length;

    return mul(vec, lengthInv);
}
