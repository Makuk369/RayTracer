#pragma once
#include <memory>
#include "glm/glm.hpp"
#include "Ray.hpp"

class Material;

class HitRecord
{
    public:
        // Sets the hit record normal vector.
        // NOTE: the parameter `outwardNormal` is assumed to have unit length.
        void SetFaceNormal(const Ray& ray, const glm::vec3& outwardNormal);
    
        glm::vec3 position{0.0f};
        glm::vec3 normal{0.0f};
        bool isFrontFace = false;
        float t = 0.0f;
        std::shared_ptr<Material> mat;
    private: 
};