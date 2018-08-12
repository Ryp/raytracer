#include "types.h"

#include <cmath>

namespace
{
    inline float2 add(const float2& v1, const float2& v2)
    {
        return {
            v1.x + v2.x,
            v1.y + v2.y
        };
    }

    inline float2 sub(const float2& v1, const float2& v2)
    {
        return {
            v1.x - v2.x,
            v1.y - v2.y
        };
    }

    inline float2 mul(const float2& v1, const float2& v2)
    {
        return {
            v1.x * v2.x,
            v1.y * v2.y
        };
    }

    inline float2 div(const float2& v1, const float2& v2)
    {
        return {
            v1.x / v2.x,
            v1.y / v2.y
        };
    }
}

float2 float2::operator+(float scalar) const
{
    return add(*this, float2{scalar, scalar});
}

float2 float2::operator+(const float2& other) const
{
    return add(*this, other);
}

float2 float2::operator-() const
{
    return sub(float2{}, *this);
}

float2 float2::operator-(float scalar) const
{
    return sub(*this, float2{scalar, scalar});
}

float2 float2::operator-(const float2& other) const
{
    return sub(*this, other);
}

float2 float2::operator*(float scalar) const
{
    return mul(*this, float2{scalar, scalar});
}

float2 float2::operator*(const float2& other) const
{
    return mul(*this, other);
}

float2 float2::operator/(float scalar) const
{
    const float scalarInv = 1.f / scalar;
    return mul(*this, float2{scalarInv, scalarInv});
}

float2 float2::operator/(const float2& other) const
{
    return div(*this, other);
}

float2::operator int2() const
{
    return int2{
        static_cast<int>(x),
        static_cast<int>(y)
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

    return vec * lengthInv;
}
