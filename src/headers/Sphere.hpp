#pragma once
#include "glm/glm.hpp"
#include "Hittable.hpp"

class Sphere : public Hittable
{
    public:
        Sphere(glm::vec3 position, float radius);

        bool Hit(const Ray& ray, const Interval& rayT, HitRecord& hitRec) const override;

    private:
        glm::vec3 mPosition;
        float mRadius;
};

