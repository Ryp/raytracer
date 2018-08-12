#include "types.h"

namespace
{
    inline float3 mul(const float3x3& mat, const float3& vec)
    {
        return float3
        {
            mat.c0.x * vec.x + mat.c1.x * vec.y + mat.c2.x * vec.z,
            mat.c0.y * vec.x + mat.c1.y * vec.y + mat.c2.y * vec.z,
            mat.c0.z * vec.x + mat.c1.z * vec.y + mat.c2.z * vec.z
        };
    }
}

float3 float3x3::operator*(const float3& other) const
{
    return mul(*this, other);
}

float3x3 lookAt(const float3& viewPosition, const float3& targetPosition, const float3& upDirection)
{
    const float3 viewForward = -normalize(targetPosition - viewPosition);
    const float3 viewRight = normalize(cross(upDirection, viewForward));
    const float3 viewUp = cross(viewForward, viewRight);

    return {
        viewRight,
        viewUp,
        viewForward
    };
}
