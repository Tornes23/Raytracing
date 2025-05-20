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
	BOX,
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
	void Destroy();

	void SetPos(const glm::vec3& pos);
	void SetRot(const glm::vec3& rotation);
	void SetSca(float scale);
	glm::mat4x4 GetModel2World() const;
	void ComputeModel2World();
	void LoadObjModel(const char* file = nullptr);

	glm::vec3 mPos{0.0F, 0.0F, 0.0F};
	glm::vec3 mRot{0.0F, 0.0F, 0.0F};
	float mScale = 1.0;
	glm::mat4x4 mM2W{ 1.0f };

	void ApplyModel2WorldToModel();
	//the corresponding Geometry
	Geometry* mModel = nullptr;
	Material* mMaterial = nullptr;
	bool mbLight = false;
};