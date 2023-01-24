#pragma once
#include <vector>
#include "Geometry.h"
#include "Object.h"

class RayTracer
{
public:

	ContactInfo CastRay(const Ray& ray, std::vector<Object>& objs);

	//singleton stuff
	RayTracer(RayTracer const&) = delete;
	void operator=(RayTracer const&) = delete;
	static RayTracer& GetInstance()
	{
		static RayTracer instance;
		return instance;
	}

private:
	RayTracer() {}

};

#define Raytracer (RayTracer::GetInstance())
