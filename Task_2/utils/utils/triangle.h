#pragma once
#include "ray.h"

struct Triangle
{
    vec3 r0, r1, r2;
};

struct RayTriangleIntersection
{
    float t;
    vec2 barycentrics;
    bool frontFacing;
};

const RayTriangleIntersection rayTriangleMiss = {0.f, vec2{-1.f, -1.f}, false};
inline bool happened(RayTriangleIntersection const i) noexcept
{
    auto const [p, q] = i.barycentrics;
    return (1.f - p - q) >= 0.f
        &&        p      >= 0.f
        &&            q  >= 0.f;
}
template<typename T>
T interpolate(T const &r, vec2 const barycentrics) noexcept
{
    auto const [p, q] = barycentrics;
    return (1.f - p - q) * r[0]
                + p      * r[1]
                    + q  * r[2];
}

inline RayTriangleIntersection rayTriangleIntersection( const Ray ray
                                                      , const Triangle tri
                                                      , const RayRange range
                                                      ) noexcept
{
    auto const [r0, r1, r2] = tri;
    vec3 const a =  r1 - r0;
    vec3 const b =  r2 - r0;
    vec3 const c = ray.origin- r0;
    vec3 const d = -ray.direction;

    f32 const det0 = dot(d, cross(a, b));

    RayTriangleIntersection const hit =
    {
        dot(c, cross(a, b)) / det0,
        {
            dot(d, cross(c, b)) / det0,
            dot(d, cross(a, c)) / det0,
        },
        det0 > 0.f,
    };

    return contains(range, hit.t)
        ? hit
        : rayTriangleMiss;
}
