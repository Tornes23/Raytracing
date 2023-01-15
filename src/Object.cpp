#include "Object.h"
#include "Geometry.h"
#include "Utils.h"


Object::Object(const char* info, GeometryTypes type)
{
	//mPos = Utils::GetVector(info);

	switch (type)
	{
	case GeometryTypes::Triangle:
		//mModel = new Triangle();
		break;
	case GeometryTypes::Plane:
		//mModel = new Plane();
		break;
	case GeometryTypes::AABB:
		//mModel = new AABB();
		break;
	case GeometryTypes::Sphere:
		//mModel = new Sphere();
		break;
	case GeometryTypes::Model:
		//mModel = new Geometry();
		break;
	default:
		//mModel = new AABB();
		break;
	}
}
void Object::Destroy()
{
	if(mModel != nullptr)
		delete mModel;
	mModel = nullptr;
}
