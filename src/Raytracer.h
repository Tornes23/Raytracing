#pragma once
#include <vector>
#include "Scene.h"

class RayTracer
{
public:

	ContactInfo FindClosestObj(const Ray& ray, const Scene& scene);
	ContactInfo RayCast(const Ray& ray, const Scene& scene, int bounce);

	void SetBounces(int bounces = 1);

	int GetBounces();
	float GetEpsilon();

#ifdef MULTITHREAD
	Ray GetRayForPixel(int x, int y);
#endif // MULTITHREAD


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
	float mEpsilon = 0.0001F;
};

#define Raytracer (RayTracer::GetInstance())
