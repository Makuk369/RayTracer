#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"
#include "headers/Sphere.hpp"

Sphere::Sphere(glm::vec3 position, float radius, std::shared_ptr<Material> mat)
    : mPosition(position), mRadius(glm::max(0.0f, radius)), mMat(mat)
{}

bool Sphere::Hit(const Ray& ray, const Interval& rayT, HitRecord& hitRec) const
{
    // Sphere formula (quadratic - modified)
	glm::vec3 oc = mPosition - ray.origin;
	float a = glm::length2(ray.direction);
	float h = glm::dot(ray.direction, oc);
	float c = glm::length2(oc) - mRadius * mRadius;

	// Quadratic forumula discriminant
	float discriminant = h * h - a * c;
	if(discriminant < 0.0f) { // ray missed
        return false;
    }
    float sqrtDiscriminant = glm::sqrt(discriminant);
    
    float root = (h - sqrtDiscriminant) / a; // closestT
    if(!rayT.Surrounds(root)){ // is outside of bounds
        root = (h + sqrtDiscriminant) / a;
        if(!rayT.Surrounds(root)){
            return false;
        }
    }

    hitRec.t = root;
    hitRec.position = ray.origin + (root * ray.direction); // hit point
    glm::vec3 outwardNormal = (hitRec.position - mPosition) / mRadius;
    hitRec.SetFaceNormal(ray, outwardNormal);
    hitRec.mat = mMat;

    return true;
}