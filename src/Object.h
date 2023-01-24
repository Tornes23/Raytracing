#pragma once
#include <memory>
#include <string>
#include <glm/vec3.hpp>

#include "Material.h"
#include "Geometry.h"

enum class GeometryTypes
{
	Triangle,
	Plane,
	AABB,
	Sphere,
	Model
};

struct Geometry;
struct Object
{
	Object(const char* info = nullptr, GeometryTypes type = GeometryTypes::AABB);
	Object(const Object& obj);
	bool CheckIntersection(const Ray& ray, ContactInfo& info);
	void Destroy();
	glm::vec3 mPos{0.0F, 0.0F, 0.0F};

	//the corresponding Geometry
	Geometry* mModel = nullptr;
	Material mMaterial;
};