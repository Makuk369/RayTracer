#pragma once
#include "glm/glm.hpp"
#include "Ray.hpp"
#include "HitRecord.hpp"

class Hittable
{
    public:
        virtual ~Hittable() = default;

        virtual bool Hit(const Ray& ray, float rayTmin, float rayTmax, HitRecord& hitRec) const = 0;

    private:
};

