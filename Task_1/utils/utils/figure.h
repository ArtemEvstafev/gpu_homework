#pragma once
#include "ray.h"
#include <algorithm>
#include <cmath>

struct Rhombus
{
    float
    la, lb,  //semi axis
    h,       // height
    ra;      // corner
};

float ndot(vec3 const a, vec3 const b) { return a.x*b.x - a.y*b.y; }
template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

inline float map(vec3 const point, Rhombus rhombus) noexcept
{
    vec3 p = abs(point);
    vec3 b = {rhombus.la, rhombus.lb, 0.f};
    vec3 p_xz = {p.x, p.z, 0.f};
    float f = std::clamp(ndot(b, b - p_xz * 2.f) / dot(b, b), -1.f, 1.f);
    vec3 q =
            {
                    .x = length(p_xz - b * vec3(1.f - f, 1.f + f, 0.f) * 0.5f) * sign(p.x * b.y + p.z * b.x - b.x * b.y) - rhombus.ra,
                    .y = p.y - rhombus.h,
                    .z = 0.f,
            };
    return std::min(std::max(q.x,q.y), 0.f) + length(max(q,0.0)) - 0.01f;
}
inline RayPoint rayRhombusIntersection(Ray const ray, Rhombus const rhombus) noexcept
{

    float t = 0;
    float const t_max = 10;
    for(int i = 0; i < 256; ++i)
    {
        vec3 point = ray.origin + ray.direction * t;
        float h = map(point, rhombus);
        if(h < 0.0001)
            break;
        if(t > t_max)
            return {1.f / 0.f};
        t += h;
    }
    return
    {
            .tMin = t,
    };
}