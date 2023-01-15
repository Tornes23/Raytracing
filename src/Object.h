#pragma once
#include <memory>
#include <string>
#include <SFML/System/Vector3.hpp>

#include "Material.h"

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
	void Destroy();
	sf::Vector3<float> mPos{0.0F, 0.0F, 0.0F};
	sf::Vector3<float> mForward{ 0.0F, 0.0F, 0.0F };
	sf::Vector3<float> mRight{ 0.0F, 0.0F, 0.0F };

	//the corresponding Geometry
	Geometry* mModel = nullptr;
	Material mMaterial;
};