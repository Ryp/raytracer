#include "types.h"

#include <cmath>

// float4

float4 add(const float4& v1, const float4& v2)
{
    return {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z,
        v1.w + v2.w
    };
}

float4 sub(const float4& v1, const float4& v2)
{
    return {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z,
        v1.w - v2.w
    };
}
