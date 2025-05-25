#include "Material.h"
#include "Utils.h"

void Diffuse::ParseData(const char** info) { mColor = Color(Utils::GetVector(info)); }

Ray Diffuse::BounceRay(const glm::vec3& incidence, const glm::vec3& normal, const glm::vec3& cp)
{
    glm::vec3 newDir = glm::normalize(normal + Utils::GetRandomVector());
    if (glm::any(glm::isnan(newDir)))
    {
        newDir = normal;
    }

    return Ray(cp + (normal * Raytracer.GetEpsilon()), newDir);
}

void Metal::ParseData(const char** info)
{
    mColor = Color(Utils::GetVector(info));
    mRoughness = Utils::GetFloat(info);
}

Ray Metal::BounceRay(const glm::vec3& incidence, const glm::vec3& normal, const glm::vec3& cp)
{
    glm::vec3 reflected = glm::reflect(incidence, normal);
    glm::vec3 candidate = reflected;

    if (mRoughness > 0.0F)
        candidate += Utils::GetRandomVector(mRoughness);

    float epsilon = Raytracer.GetEpsilon();
    if(glm::dot(candidate, candidate) < (epsilon)) return Ray(cp, glm::vec3(0.0F));
    
    glm::vec3 p = cp + (normal * Raytracer.GetEpsilon());
    glm::vec3 v = glm::normalize(candidate);
    return Ray(p, v);
}

void Dielectric::ParseData(const char** info)
{
    mColor = Color(Utils::GetVector(info));
    mIndexOfRefraction = Utils::GetInt(info);
    mAttenuation = Utils::GetVector(info);
}

Ray Dielectric::BounceRay(const glm::vec3& incidence, const glm::vec3& normal, const glm::vec3& cp)
{
    return Ray(normal, cp);
}
