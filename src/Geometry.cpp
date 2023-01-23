#include <iostream>
#include "Geometry.h"
#include "Utils.h"

Sphere::Sphere(const char** info)
{
	//parse and shi
	mRadius = Utils::GetFloat(info);
}

AABB::AABB(const char** info)
{
	mWidth = Utils::GetVector(info);
	mHeight = Utils::GetVector(info);
	mLength = Utils::GetVector(info);
}
