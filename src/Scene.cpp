#include "headers/Scene.hpp"

bool Scene::HitObjects(const Ray& ray, const Interval& rayT, HitRecord& hitRec) const
{
    HitRecord tempHitRec;
    bool hitAnything = false;
    float closestTSoFar = rayT.max;

    for (const std::shared_ptr<Hittable>& object : mObjects){
        if(object->Hit(ray, Interval(rayT.min, closestTSoFar), tempHitRec)){
            hitAnything = true;
            closestTSoFar = tempHitRec.t;
            hitRec = tempHitRec;
        }
    }
    
    return hitAnything;
}