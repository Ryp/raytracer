#include "types.h"

#include <cmath>

namespace
{
    inline float3 add(const float3& v1, const float3& v2)
    {
        return {
            v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z
        };
    }

    inline float3 sub(const float3& v1, const float3& v2)
    {
        return {
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z
        };
    }

    inline float3 mul(const float3& v1, const float3& v2)
    {
        return {
            v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z
        };
    }

    inline float3 div(const float3& v1, const float3& v2)
    {
        return {
            v1.x / v2.x,
            v1.y / v2.y,
            v1.z / v2.z
        };
    }
}

float3 float3::operator+(float scalar) const
{
    return add(*this, float3{scalar, scalar, scalar});
}

float3 float3::operator+(const float3& other) const
{
    return add(*this, other);
}

float3 float3::operator-() const
{
    return sub(float3{}, *this);
}

float3 float3::operator-(float scalar) const
{
    return sub(*this, float3{scalar, scalar, scalar});
}

float3 float3::operator-(const float3& other) const
{
    return sub(*this, other);
}

float3 float3::operator*(float scalar) const
{
    return mul(*this, float3{scalar, scalar, scalar});
}

float3 float3::operator*(const float3& other) const
{
    return mul(*this, other);
}

float3 float3::operator/(float scalar) const
{
    const float scalarInv = 1.f / scalar;
    return mul(*this, float3{scalarInv, scalarInv, scalarInv});
}

float3::operator int3() const
{
    return int3{
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(z)
    };
}

float3 float3::operator/(const float3& other) const
{
    return div(*this, other);
}

float lengthSqr(const float3& vec)
{
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

float3 normalize(const float3& vec)
{
    const float length = sqrt(lengthSqr(vec));
    const float lengthInv = 1.f / length;

    return vec * lengthInv;
}

float3 pow(const float3& vec, float scalar)
{
    return {
        powf(vec.x, scalar),
        powf(vec.y, scalar),
        powf(vec.z, scalar)
    };
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

// normal should be normalized
float3 reflect(const float3& vec, const float3& normal)
{
    return vec - normal * 2.f * dot(vec, normal);
}

float3 saturate(const float3& vec)
{
    return {
        fmaxf(fminf(vec.x, 1.f), 0.f),
        fmaxf(fminf(vec.y, 1.f), 0.f),
        fmaxf(fminf(vec.z, 1.f), 0.f)
    };
}
