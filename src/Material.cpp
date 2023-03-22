#include "Material.h"
#include "Utils.h"

void Diffuse::ParseData(const char** info) { mColor = Color(Utils::GetVector(info)); }

Ray Diffuse::BounceRay(const glm::vec3& incidence, const glm::vec3& normal, const glm::vec3& cp)
{
    return Ray(cp + (normal * Raytracer.GetEpsilon()), normal + Utils::GetRandomVector());
}

void Metal::ParseData(const char** info)
{
    mColor = Color(Utils::GetVector(info));
    mRoughness = Utils::GetFloat(info);
}

Ray Metal::BounceRay(const glm::vec3& incidence, const glm::vec3& normal, const glm::vec3& cp)
{
    glm::vec3 reflected = glm::reflect(incidence, normal);
    glm::vec3 candidate = reflected + (Utils::GetRandomVector() * mRoughness);

    if(glm::dot(candidate, normal) < 0) candidate = reflected + (Utils::GetRandomVector() * mRoughness);
    
    candidate = glm::normalize(candidate);

    glm::vec3 p = cp + (candidate * Raytracer.GetEpsilon());
    glm::vec3 v = candidate;
    return Ray(v, p);
}

void Dielectric::ParseData(const char** info)
{
}

Ray Dielectric::BounceRay(const glm::vec3& incidence, const glm::vec3& normal, const glm::vec3& cp)
{
    return Ray(normal, cp);
}
