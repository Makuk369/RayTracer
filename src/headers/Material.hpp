#pragma once

#include "glm/glm.hpp"
#include "Ray.hpp"
#include "HitRecord.hpp"

class Material
{
    public:
        virtual ~Material() = default;

        // virtual bool Scatter(Ray& ray, const HitRecord& hitRec, glm::vec3& color) const { return false; }
        virtual bool Scatter(const Ray& ray, const HitRecord& hitRec, glm::vec4& color, Ray& scatteredRay) const { return false; }

    private:
};

class Lambertian : public Material
{
    public:
        Lambertian(const glm::vec4& albedo);

        // bool Scatter(Ray& ray, const HitRecord& hitRec, glm::vec3& color) const override;
        bool Scatter(const Ray& ray, const HitRecord& hitRec, glm::vec4& color, Ray& scatteredRay) const override;

    private:
        glm::vec4 mAlbedo;
};

class Metal : public Material
{
    public:
        Metal(const glm::vec4& albedo, float roughness);

        // bool Scatter(Ray& ray, const HitRecord& hitRec, glm::vec3& color) const override;
        bool Scatter(const Ray& ray, const HitRecord& hitRec, glm::vec4& color, Ray& scatteredRay) const override;

    private:
        glm::vec4 mAlbedo;
        float mRoughness;
};
