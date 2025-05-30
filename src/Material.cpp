#include "Material.h"
#include "Utils.h"

void Diffuse::ParseData(const char** info) { mColor = Color(Utils::GetVector(info)); }

Ray Diffuse::BounceRay(const Ray& ray, const glm::vec3& normal, const glm::vec3& cp)
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

Ray Metal::BounceRay(const Ray& ray, const glm::vec3& normal, const glm::vec3& cp)
{
    glm::vec3 incidence = ray.mV;
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
    mIndexOfRefraction = Utils::GetFloat(info);
    mAttenuation = Utils::GetVector(info);
}

Ray Dielectric::BounceRay(const Ray& ray, const glm::vec3& normal, const glm::vec3& cp)
{
	glm::vec3 p = cp + (normal * Raytracer.GetEpsilon());
	Ray bounced(p);
	bounced.mAttenuation= ray.mAttenuation;
	bounced.mIndexOfRefraction = ray.mIndexOfRefraction;


	glm::vec3 incidence = ray.mV;
    float randFloat = Utils::GetRandomFloat(0.0f, 1.0f);

	// If we are inside of a medium different from air
	float ni = ray.mIndexOfRefraction;
	float nt = mIndexOfRefraction;
	if (ray.mIndexOfRefraction != 1.0f)
	{
		ni = mIndexOfRefraction;
		nt = 1.0f;
	}

	float ni_nt = ni / nt;
	float ni_ntSq = ni_nt * ni_nt;

	float cosi = glm::dot(-incidence, normal);

	float discriminant = 1.0f - ni_ntSq * (1.0f - cosi * cosi);

	// Perfect reflection case
	if (discriminant < 0)
	{
		bounced.mV = glm::normalize(glm::reflect(incidence, normal));
		return bounced;
	}

	discriminant = sqrtf(discriminant);

	float erper = ni * cosi - nt * discriminant;
	float eiper = ni * cosi + nt * discriminant;

	float erpar = nt * cosi - ni * discriminant;
	float eipar = nt * cosi + ni * discriminant;

	float rper = erper / eiper;
	float rpar = erpar / eipar;

	float R = 0.5f * (rper * rper + rpar * rpar);
	float T = 1.0f - R;

	// Reflect
	if (randFloat < (1.0f - T))
	{
		bounced.mV = glm::normalize(glm::reflect(incidence, normal));
		return bounced;
	}
	else// Refract
	{
		if (ray.mIndexOfRefraction != mIndexOfRefraction)
		{
			bounced.mAttenuation = mAttenuation;
			bounced.mIndexOfRefraction = mIndexOfRefraction;
		}
		else
		{
			bounced.mAttenuation = glm::vec3(1.0f);
			bounced.mIndexOfRefraction = 1.0f;
		}
		
		
		bounced.mV = glm::normalize(glm::refract(incidence, normal, ni_nt));
		return bounced;
	}

    return bounced;
}
