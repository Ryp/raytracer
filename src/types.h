#pragma once

// Integer

struct float2;

struct int2
{
    int x;
    int y;

    explicit operator float2() const;
};

struct int3
{
    int x;
    int y;
    int z;

    int3 operator+(int scalar) const;
    int3 operator+(const int3& other) const;
    int3 operator-() const;
    int3 operator-(int scalar) const;
    int3 operator-(const int3& other) const;
    int3 operator*(int scalar) const;
    int3 operator*(const int3& other) const;
    int3 operator/(int scalar) const;
    int3 operator/(const int3& other) const;
};

// Floating point

struct float2
{
    float x;
    float y;

    float2 operator+(float scalar) const;
    float2 operator+(const float2& other) const;
    float2 operator-() const;
    float2 operator-(float scalar) const;
    float2 operator-(const float2& other) const;
    float2 operator*(float scalar) const;
    float2 operator*(const float2& other) const;
    float2 operator/(float scalar) const;
    float2 operator/(const float2& other) const;

    explicit operator int2() const;
};

float lengthSqr(const float2& vec);
float2 normalize(const float2& vec);

struct float3
{
    float x;
    float y;
    float z;

    float3 operator+(float scalar) const;
    float3 operator+(const float3& other) const;
    float3 operator-() const;
    float3 operator-(float scalar) const;
    float3 operator-(const float3& other) const;
    float3 operator*(float scalar) const;
    float3 operator*(const float3& other) const;
    float3 operator/(float scalar) const;
    float3 operator/(const float3& other) const;

    explicit operator int3() const;
};

float lengthSqr(const float3& vec);
float3 normalize(const float3& vec);
float dot(const float3& v1, const float3& v2);
float3 pow(const float3& vec, float scalar);
float3 cross(const float3& v1, const float3& v2);
float3 saturate(const float3& vec);
float3 reflect(const float3& vec, const float3& normal);

struct float3x3
{
    float3 c0;
    float3 c1;
    float3 c2;

    float3 operator*(const float3& other) const;
};

float3x3 lookAt(const float3& viewPosition, const float3& targetPosition, const float3& upDirection);
