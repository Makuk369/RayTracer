#include "headers/Material.hpp"
#include "headers/Utils.hpp"

Lambertian::Lambertian(const glm::vec3& albedo)
    : mAlbedo(albedo)
{}


bool Lambertian::Scatter(Ray& ray, const HitRecord& hitRec, glm::vec3& color) const
{
    glm::vec3 scatterDir = hitRec.normal + RTUtils::RandomUnitVec3();

    // Catch degenerate scatter direction
    if(RTUtils::isNearZeroVec(scatterDir))
    {
        scatterDir = hitRec.normal;
    }

    ray.origin = hitRec.position + hitRec.normal * 0.0001f;
    ray.direction = scatterDir;
    color = mAlbedo;

    return true;
}

bool Lambertian::Scatter(const Ray& ray, const HitRecord& hitRec, glm::vec3& color, Ray& scatteredRay) const
{
    glm::vec3 scatterDir = hitRec.normal + RTUtils::RandomUnitVec3();

    // Catch degenerate scatter direction
    if(RTUtils::isNearZeroVec(scatterDir))
    {
        scatterDir = hitRec.normal;
    }

    scatteredRay.origin = hitRec.position + hitRec.normal * 0.0001f;
    scatteredRay.direction = scatterDir;
    color = mAlbedo;

    return true;
}

Metal::Metal(const glm::vec3& albedo, float roughness)
    : mAlbedo(albedo), mRoughness(roughness < 1.0f ? roughness : 1.0f)
{}


bool Metal::Scatter(Ray& ray, const HitRecord& hitRec, glm::vec3& color) const
{
    glm::vec3 reflectDir = glm::reflect(ray.direction, hitRec.normal);
    reflectDir = glm::normalize(reflectDir) + (mRoughness * RTUtils::RandomUnitVec3());

    ray.origin = hitRec.position + hitRec.normal * 0.0001f;
    ray.direction = reflectDir;
    color = mAlbedo;

    return (glm::dot(ray.direction, hitRec.normal) > 0); // false if ray.direction points into the sphere
}

bool Metal::Scatter(const Ray& ray, const HitRecord& hitRec, glm::vec3& color, Ray& scatteredRay) const
{
    glm::vec3 reflectDir = glm::reflect(ray.direction, hitRec.normal);
    reflectDir = glm::normalize(reflectDir) + (mRoughness * RTUtils::RandomUnitVec3());

    scatteredRay.origin = hitRec.position + hitRec.normal * 0.0001f;
    scatteredRay.direction = reflectDir;
    color = mAlbedo;

    return (glm::dot(scatteredRay.direction, hitRec.normal) > 0);
}