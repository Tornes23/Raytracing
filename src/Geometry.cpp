#include <iostream>
#include <glm/gtx/transform.hpp>
#include "Geometry.h"
#include "Utils.h"

Sphere::Sphere(const char** info)
{
	//parse and shi
	mRadius = Utils::GetFloat(info);
}

bool Sphere::CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info)
{
	float a = glm::dot(ray.mV, ray.mV);
	glm::vec3 cp = ray.mP0 - center;
	float b = 2.0F * glm::dot(ray.mV, cp);
	float c = glm::dot(cp, cp) - (mRadius * mRadius);
	float discriminant = (b * b) - 4.0F * a * c;

	if (a == 0.0F) return false;
	if (discriminant < 0.0F) return false;

	info.mT0 = (-b - glm::sqrt(discriminant)) / 2.0f * a;
	info.mT1 = (-b + glm::sqrt(discriminant)) / 2.0f * a;

	if (info.mT0 < 0.0F && info.mT1 < 0.0F) return false;

	if (info.mT0 >= 0.0F && info.mT1 >= 0.0F)
		info.mTI = glm::min(info.mT0, info.mT1);//probablu can be changed to t0 sin is going to be lower always
	if (info.mT0 >= 0.0F && info.mT1 < 0.0F)
		info.mTI = info.mT0;
	if (info.mT0 < 0.0F && info.mT1 >= 0.0F)
		info.mTI = info.mT1;

	info.mContact = ray.mP0 + ray.mV * info.mTI;
	info.mNomal = glm::normalize(info.mContact - center);

	return true;
}

AABB::AABB(const char** info)
{
	mWidth = Utils::GetVector(info);
	mHeight = Utils::GetVector(info);
	mLength = Utils::GetVector(info);
}

bool AABB::CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info)
{
	return false;
}
