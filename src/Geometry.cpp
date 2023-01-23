#include <iostream>
#include "Geometry.h"
#include "Utils.h"

Sphere::Sphere(const char* info)
{
	//parse and shi
	std::cout << "Tetsing";
	mRadius = Utils::GetFloat(info, 0, std::strlen(info));
}
