#pragma once
#include "ray.h"

struct AABB
{
    vec3 min, max;
};

inline AABB mergeBox(AABB const box1, AABB const box2) noexcept
{
    return
    {
        .min = min(box1.min, box2.min),
        .max = max(box1.max, box2.max),
    };
}

inline RayRange rayAABBIntersection( const Ray ray
                                   , const AABB box
                                   , const RayRange range
                                   ) noexcept
{
    vec3 const a = (box.min - ray.origin) / ray.direction;
    vec3 const b = (box.max - ray.origin) / ray.direction;
    vec3 const tmin = min(a, b);
    vec3 const tmax = max(a, b);

    auto const min2 = [](f32 const x, f32 const y) noexcept {return x < y ? x : y;};
    auto const max2 = [](f32 const x, f32 const y) noexcept {return x > y ? x : y;};
    return
    {
        max2(tmin[0], max2(tmin[1], max2(tmin[2], range.tMin))),
        min2(tmax[0], min2(tmax[1], min2(tmax[2], range.tMax))),
    };
}
