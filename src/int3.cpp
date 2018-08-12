#include "types.h"

#include <cmath>

int2::operator float2() const
{
    return float2{
        static_cast<float>(x),
        static_cast<float>(y)
    };
}

namespace
{
    inline int3 add(const int3& v1, const int3& v2)
    {
        return {
            v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z
        };
    }

    inline int3 sub(const int3& v1, const int3& v2)
    {
        return {
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z
        };
    }

    inline int3 mul(const int3& v1, const int3& v2)
    {
        return {
            v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z
        };
    }

    inline int3 div(const int3& v1, const int3& v2)
    {
        return {
            v1.x / v2.x,
            v1.y / v2.y,
            v1.z / v2.z
        };
    }
}

int3 int3::operator+(int scalar) const
{
    return add(*this, int3{scalar, scalar, scalar});
}

int3 int3::operator+(const int3& other) const
{
    return add(*this, other);
}

int3 int3::operator-() const
{
    return sub(int3{}, *this);
}

int3 int3::operator-(int scalar) const
{
    return sub(*this, int3{scalar, scalar, scalar});
}

int3 int3::operator-(const int3& other) const
{
    return sub(*this, other);
}

int3 int3::operator*(int scalar) const
{
    return mul(*this, int3{scalar, scalar, scalar});
}

int3 int3::operator*(const int3& other) const
{
    return mul(*this, other);
}

int3 int3::operator/(int scalar) const
{
    return div(*this, int3{scalar, scalar, scalar});
}

int3 int3::operator/(const int3& other) const
{
    return div(*this, other);
}
