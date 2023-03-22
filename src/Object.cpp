#include <glm/gtx/transform.hpp>
#include "Object.h"
#include "Geometry.h"
#include "Utils.h"
#include "Material.h"


Object::Object(const char* info, GeometryTypes type)
{
	if (info == nullptr)
		return;
	if(type != GeometryTypes::Polygon && type != GeometryTypes::Model)
		mPos = Utils::GetVector(&info);

	glm::mat4x4 m2w = glm::mat4x4(1.0);
	m2w = glm::translate(m2w, mPos);
	m2w = m2w * glm::rotate(glm::radians(mRot.x), glm::vec3(1, 0, 0));
	m2w = m2w * glm::rotate(glm::radians(mRot.y), glm::vec3(0, 1, 0));
	m2w = m2w * glm::rotate(glm::radians(mRot.z), glm::vec3(0, 0, 1));
	m2w = m2w * glm::scale(glm::vec3(mScale));

	switch (type)
	{
	case GeometryTypes::Triangle:
		//mModel = new Triangle();
		break;
	case GeometryTypes::Plane:
		//mModel = new Plane();
		break;
	case GeometryTypes::BOX:
		mModel = new Box(&info);
		break;
	case GeometryTypes::AABB:
		mModel = new AABB(&info);
		break;
	case GeometryTypes::Sphere:
		mModel = new Sphere(&info);
		break;
	case GeometryTypes::Polygon:
		mModel = new Polygon(&info);
		break;
	case GeometryTypes::Model:
		mModel = new Model(&info, m2w);
		break;
	default:
		mModel = new Sphere(&info);
		break;
	}
	mbLight = false;
}

Object::Object(const Object& obj)
{
	mPos = obj.mPos;
	mModel = obj.mModel;
	mMaterial = obj.mMaterial;
	mbLight = obj.mbLight;
}

Object::Object(const Light& light)
{
	mModel = new Sphere(light.mRadius);
	mPos = light.mPos;
	mMaterial = new Diffuse;
	mMaterial->mColor = light.mColor;
	mbLight = true;
}

bool Object::CheckIntersection(const Ray& ray, ContactInfo& info)
{
	bool intersected = mModel->CheckIntersection(ray, mPos, info);

	if (!intersected)
		return false;

	if(mMaterial != nullptr)
		info.mColor = mMaterial->mColor;

	return intersected;
}

void Object::Destroy()
{
	if (mModel != nullptr)
	{
		delete mModel;
		mModel = nullptr;
	}
	if (mMaterial != nullptr)
	{
		delete mMaterial;
		mMaterial = nullptr;
	}
}

void Object::SetPos(const glm::vec3& pos) { mPos = pos; }
void Object::SetRot(const glm::vec3& rotation) { mRot = rotation; }
void Object::SetSca(float scale) { mScale = scale; }
