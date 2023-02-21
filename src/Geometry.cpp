#include <iostream>
#include <TinyOBJ/tiny_obj_loader.h>
#include <glm/gtx/transform.hpp>
#include "Geometry.h"
#include "Utils.h"
#include "GraphicsManager.h"

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
	info.mNormal = glm::normalize(info.mContact - center);

	if (glm::dot(info.mNormal, ray.mV) > 0)
		info.mNormal = -info.mNormal;

	return true;
}

AABB::AABB(const glm::vec3& length, const glm::vec3& width, const glm::vec3& height)
{
	mVectors.resize(3);

	mVectors[0] = length;
	mVectors[1] = width;
	mVectors[2] = height;

}

AABB::AABB(const char** info)
{
	mVectors.resize(3);

	mVectors[0] = Utils::GetVector(info);//length
	mVectors[1] = Utils::GetVector(info);//width
	mVectors[2] = Utils::GetVector(info);//height
}

bool AABB::CheckIntersection(const Ray& ray, const glm::vec3& corner, ContactInfo& info)
{
	bool all = true;
	glm::vec2 mainInterval(0.0F, std::numeric_limits<float>::max());

	glm::vec3 cp = glm::normalize(ray.mP0 - corner);
	std::vector<Plane> planes(6);
	std::vector<glm::vec3> displacements(6);

	planes[0] = Plane(glm::normalize(glm::cross(mVectors[0], mVectors[2])));//front plane
	planes[1] = Plane(-planes[0].mNormal);// back plane
	planes[2] = Plane(glm::normalize(glm::cross(mVectors[2], mVectors[1])));//left plane
	planes[3] = Plane(-planes[2].mNormal);//right plane
	planes[4] = Plane(glm::normalize(glm::cross(mVectors[1], mVectors[0])));// bot plane
	planes[5] = Plane(-planes[4].mNormal);//top plane

	displacements[0] = glm::vec3(0.0F);//front plane
	displacements[1] = mVectors[1];// back plane
	displacements[2] = glm::vec3(0.0F);//left plane
	displacements[3] = mVectors[0];// right plane
	displacements[4] = glm::vec3(0.0F);//bottom plane
	displacements[5] = mVectors[2];// top plane

	ContactInfo temp;
	int indexMin = 0;
	int indexMax = 0;
	for (int i = 0; i < 6; i++)
	{
		glm::vec2 interval(0.0F, std::numeric_limits<float>::max());
		float raydot = glm::dot(ray.mV, planes[i].mNormal);
		glm::vec3 point = corner + displacements[i];
		if (raydot == 0.0F || !planes[i].CheckIntersection(ray, point, interval))
		{
			all = false;
			break;
		}

		if (mainInterval.x < interval.x)
			indexMin = i;

		mainInterval.x = glm::max(mainInterval.x, interval.x);
		mainInterval.y = glm::min(mainInterval.y, interval.y);

		if (interval.x == 0.0f && mainInterval.y < interval.y)
			indexMax = i;

		if (mainInterval.y < mainInterval.x)
		{
			all = false;
			break;
		}

	}

	if(!all)
		return false;

	if (mainInterval.x == 0.0F)
	{
		info.mTI = mainInterval.y;
		info.mNormal = planes[indexMax].mNormal;
	}
	else
	{
		info.mTI = mainInterval.x;
		info.mNormal = planes[indexMin].mNormal;
	}
	
	info.mContact = ray.mP0 + info.mTI * ray.mV;


	if (glm::dot(info.mNormal, ray.mV) > 0)
		info.mNormal = -info.mNormal;


	return true;
}

bool Plane::CheckIntersection(const Ray& ray, const glm::vec3& point, glm::vec2& interval)
{
	float raydot = glm::dot(ray.mV, mNormal);
	float epsilon = std::numeric_limits<float>::epsilon();
	if(raydot < std::numeric_limits<float>::epsilon() && -std::numeric_limits<float>::epsilon() < raydot)
		return false;

	//glm::vec3 cp = glm::normalize(ray.mP0 - point);
	glm::vec3 cp = ray.mP0 - point;
	float dot = glm::dot(cp, mNormal);
	float time = -dot / raydot;

	if (raydot <= 0.0F)
	{
		if (dot >= 0.0F)
			interval.x = time;
	}
	else
	{
		if (dot < 0.0F)
			interval.y = time;
		else
			return false;
	}

	return true;
}

bool Plane::CheckIntersection(const Ray& ray, const glm::vec3& point, ContactInfo& info)
{
	float dot = glm::dot(ray.mV, mNormal);
	if (dot == 0.0F)
		return false;

	glm::vec3 cp = glm::normalize(ray.mP0 - point);
	float time = -glm::dot(cp, mNormal) / dot;

	if (time < 0.0F)
		return false;

	info.mTI = time;
	info.mContact = ray.mP0 + time * ray.mV;
	info.mNormal = mNormal;

	return true;
}

Ray::Ray(const glm::vec3& p0, const glm::vec3& vec)
{
	mP0 = p0;
	mV = glm::normalize(vec);
}

Polygon::Polygon(const char** info)
{
	int vertices = Utils::GetInt(info);

	mVertices.resize(vertices);
	for (int i = 0; i < vertices; i++)
		mVertices[i] = Utils::GetVector(info);

	//triangulate
}

bool Polygon::CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info)
{
	return false;
}

Model::Model(const char** info)
{
	std::string file = *info;
	mModel = std::make_shared<Mesh>(Mesh(file));
}

bool Model::CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info)
{
	return false;
}

Mesh::Mesh(const std::string& obj)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	//bool valid = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, obj.c_str());

	//triangulate
}

bool ContactInfo::IsValid() { return (mT0 >= 0.0F && mT1 >= 0.0F && mTI >= 0.0F); }
