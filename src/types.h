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
};

struct float3
{
    float x;
    float y;
    float z;
};

struct float4
{
    float x;
    float y;
    float z;
    float w;
};

// float3
float3 add(const float3& v1, const float3& v2);
float3 sub(const float3& v1, const float3& v2);
float3 mul(const float3& vec, float scalar);
float3 normalize(const float3& vec);
float dot(const float3& v1, const float3& v2);
float3 cross(const float3& v1, const float3& v2);

// float4
float4 add(const float4& v1, const float4& v2);
float4 sub(const float4& v1, const float4& v2);
