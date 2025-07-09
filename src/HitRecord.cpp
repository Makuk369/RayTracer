#include "headers/HitRecord.hpp"

void HitRecord::SetFaceNormal(const Ray& ray, const glm::vec3& outwardNormal)
{
    isFrontFace = glm::dot(ray.direction, outwardNormal) < 0;
    normal = isFrontFace ? outwardNormal : -outwardNormal;
}