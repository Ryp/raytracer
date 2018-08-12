#pragma once

// Integer

struct int2
{
    int x;
    int y;
};

struct int3
{
    int x;
    int y;
    int z;
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
};

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
};

// float2
float lengthSqr(const float2& vec);
float2 normalize(const float2& vec);

// float3
float lengthSqr(const float3& vec);
float3 normalize(const float3& vec);
float dot(const float3& v1, const float3& v2);
float3 cross(const float3& v1, const float3& v2);
float3 saturate(const float3& vec);
float3 reflect(const float3& vec, const float3& normal);
