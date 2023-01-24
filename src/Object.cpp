#include "Object.h"
#include "Geometry.h"
#include "Utils.h"


Object::Object(const char* info, GeometryTypes type)
{
	if (info == nullptr)
		return;

	mPos = Utils::GetVector(&info);

	switch (type)
	{
	case GeometryTypes::Triangle:
		//mModel = new Triangle();
		break;
	case GeometryTypes::Plane:
		//mModel = new Plane();
		break;
	case GeometryTypes::AABB:
		mModel = new AABB(&info);
		break;
	case GeometryTypes::Sphere:
		mModel = new Sphere(&info);
		break;
	case GeometryTypes::Model:
		//mModel = new Geometry();
		break;
	default:
		mModel = new AABB(&info);
		break;
	}

}

Object::Object(const Object& obj)
{
	mPos = obj.mPos;
	mModel = obj.mModel;
	mMaterial = obj.mMaterial;
}

bool Object::CheckIntersection(const Ray& ray, ContactInfo& info)
{
	return mModel->CheckIntersection(ray, mPos, info);
}

void Object::Destroy()
{
	if (mModel != nullptr)
	{
		delete mModel;
		mModel = nullptr;
	}
}
