#pragma once
#include "ray.h"

struct Triangle
{
    vec3 r0, r1, r2;
};

inline RayPoint rayTriangleIntersection(Ray const ray, Triangle const triangle) noexcept
{
    vec3 const e1 = triangle.r1 - triangle.r0;
    vec3 const e2 = triangle.r2 - triangle.r0;
    vec3 const c  = ray.origin - triangle.r0;

    float const det0 = dot(-ray.direction, cross(e1, e2));

    float const t  = dot( c, cross(e1, e2)) / det0;
    float const b1 = dot(-ray.direction, cross(c, e2)) / det0;
    float const b2 = dot(-ray.direction, cross(e1, c)) / det0;
    if(b1 >= 0 && b2 >= 0 && 1 - b1 - b2 >= 0){
        return
                {
                        .tMin = t,
                };
    }else{
        return
                {
                        .tMin = 1.f / 0.f,
                };
    }

}