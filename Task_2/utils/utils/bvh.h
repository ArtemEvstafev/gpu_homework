#pragma once
#include <memory>
#include "aabb.h"

template<typename T>
struct BVHNode
{
    struct Kids
    {
        std::unique_ptr<BVHNode<T>> left, right;
    };

    AABB box;
    std::variant<Kids, T> data;
};
template<typename T>
using BVH = std::unique_ptr<BVHNode<T>>;
