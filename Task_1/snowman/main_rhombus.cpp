#include <algorithm>
#include <fstream>
#include <ranges>
#include <cmath>

#include <iostream>

#include <utils/camera.h>
#include <utils/figure.h>

int main()
{
    Rhombus const figures[1] =
            {
            {0.6f, 0.2f, 0.02f, 0.02f},
            };
    vec3 const albedo[1] =
            {
            {10.f, 0.f, 0.f}
            };

    struct Hit
    {
        u32 i;
        f32 t;
    };
    auto const closestHit = [figures](Ray const ray) noexcept
            -> std::optional<Hit>
    {
        auto const intersect = [ray](Rhombus const &s) noexcept {return rayRhombusIntersection(ray, s);};
        auto const intersectionR = figures | std::views::transform(intersect);
        auto const bad = [](RayPoint const r) noexcept {return !std::isfinite(r.tMin);};
        auto const less = [=](RayPoint const r1, RayPoint const r2) noexcept
        {
            if(bad(r1))
                return false;
            if(bad(r2))
                return true;
            return r1.tMin < r2.tMin;
        };
        auto const closestI = std::ranges::min_element(intersectionR, less);
        if(bad(*closestI)) {
            return std::nullopt;
        }
        return Hit
                {
                        u32(closestI - std::ranges::begin(intersectionR)),
                        (*closestI).tMin
                };
    };

    auto const trace = [&](Ray const ray) noexcept
            -> vec3
    {
        vec3 const   skyColor = {0.53f, 0.81f, 0.92f};
        vec3 const lightColor = {1.00f, 0.98f, 0.88f};
        vec3 const lightDir   = normalize({3.f, 3.f, -1.f});

        auto const hit = closestHit(ray);
        if(!hit)
        {
            return skyColor;
        }

        auto const [i, t] = *hit;
        vec3 const pos = ray.origin + ray.direction * t;

        auto const calcNormal = [&](vec3 const pos)
        {
            vec3 const e = vec3(1.f,-1.f, 0.f) * 0.5773f;
            const float eps = 0.0005f;
            vec3 const e_xyy = {e.x, e.y, e.y};
            vec3 const e_yyx = {e.y, e.y, e.x};
            vec3 const e_yxy = {e.y, e.x, e.y};
            vec3 const e_xxx = {e.x, e.x, e.x};
            return normalize( e_xyy * map(pos + e_xyy*eps, figures[i]) +
                              e_yyx * map(pos + e_yyx*eps, figures[i]) +
                              e_yxy * map(pos + e_yxy*eps, figures[i]) +
                              e_xxx * map(pos + e_xxx*eps, figures[i]) );
        };
        vec3 const norm = calcNormal(pos);
        float const NL = std::max(0.f, dot(norm, lightDir));

        auto const shadowHit = closestHit({pos + lightDir * 0.001f, lightDir});
//        auto shadowHit = false;;
        return albedo[i] * (skyColor * 0.1f + lightColor * NL * (shadowHit ? 0.f : 0.7f));
//        vec3 albedo = {10.f, 0.f, 0.f};
//        if(i == 0)
//            albedo = {0.f, 5.f, 0.f};
//        return albedo * (skyColor * 0.1f + lightColor * NL * (shadowHit ? 0.f : 0.7f));
    };

    std::ofstream file("out.ppm");
    u32 const width  = 1920u;
    u32 const height = 1080u;
    file << "P3\n" << width << ' ' << height << "\n255\n";

    Camera const camera =
            {
                    .origin = {0.f, 0.4f, 0.7f},
                    .at = {0.f, 0.f, 0.f},
                    .up = {0.f, 1.f, 0.f},
                    .fov = 0.55f,
                    .aspectRatio = f32(width) / f32(height),
            };

    for(u32 y = 0u; y < height; ++y)
        for(u32 x = 0u; x <  width; ++x)
        {
            f32 const u = -1.f + 2.f * (0.5f + f32(x)) / f32( width);
            f32 const v =  1.f - 2.f * (0.5f + f32(y)) / f32(height);
            auto const [r, g, b] = trace(camera.castRay(u, v));

            auto const encode = +[](f32 const x_) noexcept
            {
                f32 const f = x_ < 0.0031308f
                              ? x_ * 12.92f
                              : 1.055f * std::pow(x_, 1.f / 2.4f) - 0.055f;
                return u32(std::round(255.f * f));
            };
            file << encode(r) << ' '
                 << encode(g) << ' '
                 << encode(b) << ' ';
        }
}
