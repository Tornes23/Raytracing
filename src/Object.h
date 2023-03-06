#pragma once
#include <memory>
#include <string>
#include <glm/vec3.hpp>

#include "Light.h"
#include "Geometry.h"

enum class GeometryTypes
{
	Triangle,
	Plane,
	AABB,
	Sphere,
	Polygon,
	Model
};

struct Geometry;
struct Material;
struct Object
{
	Object(const char* info = nullptr, GeometryTypes type = GeometryTypes::AABB);
	Object(const Object& obj);
	Object(const Light& light);
	bool CheckIntersection(const Ray& ray, ContactInfo& info);
	bool CheckRayPath(const Ray& ray, ContactInfo& info);
	void Destroy();
	glm::vec3 mPos{0.0F, 0.0F, 0.0F};

	//the corresponding Geometry
	Geometry* mModel = nullptr;
	Material* mMaterial = nullptr;
	bool mbLight = false;
};