#pragma once
#include <memory>
#include <vector>
#include "Hittable.hpp"

class Scene
{
    public:
        void Clear() { mObjects.clear(); };
        void Add(std::shared_ptr<Hittable> object) { mObjects.push_back(object); };

        bool HitObjects(const Ray& ray, float rayTmin, float rayTmax, HitRecord& hitRec) const;

    private:
        std::vector<std::shared_ptr<Hittable>> mObjects;
};

