#pragma once
#include "glm/glm.hpp"
#include "Ray.hpp"
#include "HitRecord.hpp"
#include "Interval.hpp"

class Hittable
{
    public:
        virtual ~Hittable() = default;

        virtual bool Hit(const Ray& ray, const Interval& rayT, HitRecord& hitRec) const = 0;

    private:
};

