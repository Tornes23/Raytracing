#pragma once
#include <vector>
#include "Geometry.h"
#include "Object.h"

class RayTracer
{
public:

	ContactInfo CastRay(const Ray& ray, std::vector<Object>& objs);
	void SetBounces(int bounces = 1);
	void SetSamples(int samples = 1);

	int GetBounces();
	int GetSamples();

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
	int mBounces = 1;
	int mSamples = 1;
};

#define Raytracer (RayTracer::GetInstance())
