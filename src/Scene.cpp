#include "headers/Scene.hpp"

bool Scene::HitObjects(const Ray& ray, float rayTmin, float rayTmax, HitRecord& hitRec) const
{
    HitRecord tempHitRec;
    bool hitAnything = false;
    float closestTSoFar = rayTmax;

    for (const std::shared_ptr<Hittable>& object : mObjects){
        if(object->Hit(ray, rayTmin, closestTSoFar, tempHitRec)){
            hitAnything = true;
            closestTSoFar = tempHitRec.t;
            hitRec = tempHitRec;
        }
    }
    
    return hitAnything;
}