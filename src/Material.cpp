#include "Material.h"
#include "Utils.h"

void Diffuse::ParseData(const char** info) { mColor = Color(Utils::GetVector(info)); }

Ray Diffuse::BounceRay(const glm::vec3& normal, const glm::vec3& cp)
{
    return Ray(cp + (normal * Raytracer.GetEpsilon()), normal + Utils::GetRandomVector());
}

void Metal::ParseData(const char** info)
{
    mColor = Color(Utils::GetVector(info));
    mRoughness = Utils::GetFloat(info);
}

Ray Metal::BounceRay(const glm::vec3& normal, const glm::vec3& cp)
{

    return Ray(normal, cp);
}

void Dielectric::ParseData(const char** info)
{
}

Ray Dielectric::BounceRay(const glm::vec3& normal, const glm::vec3& cp)
{
    return Ray(normal, cp);
}
