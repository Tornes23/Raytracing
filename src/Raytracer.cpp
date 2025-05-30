#include <glm/gtx/transform.hpp>
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "Raytracer.h"
#include "Object.h"
#include "Utils.h"
#include "KDTree.hpp"

ContactInfo RayTracer::FindClosestObj(const Ray& ray, const Scene& scene)
{
    ContactInfo minInfo;
    minInfo.mTI = std::numeric_limits<float>::max();
    int minIndex = -1;
    const std::vector<Object>& objects = scene.mObjects;

    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i].mGeometryType == GeometryTypes::Model)
        {
            continue;
        }

        ContactInfo info;
        bool intersected = objects[i].CheckIntersection(ray, info);

        if (intersected)
        {
            if (info.mTI < minInfo.mTI){
                minInfo = info;
                minIndex = i;
                minInfo.mCollidedWith = &objects[minIndex];
            }
        }
    }

	kdtree::intersection treeIntersection = scene.mKDTree.get_closest(ray, nullptr);
	if (treeIntersection.t >= 0.0f && treeIntersection.t < minInfo.mTI)
	{
        minInfo.mTI = treeIntersection.t;

        size_t index = treeIntersection.triangle_index;
		const kdtree::triangle_wrapper& triangleWrapper = scene.mKDTree.triangles()[index];
        minInfo.mNormal = triangleWrapper.tri.mNormal;
        minInfo.mColor = triangleWrapper.owner && triangleWrapper.owner->mMaterial ? triangleWrapper.owner->mMaterial->mColor : Color::Black;

		if (glm::dot(minInfo.mNormal, ray.mV) > 0)
			minInfo.mNormal = -minInfo.mNormal;

        minInfo.mCollidedWith = triangleWrapper.owner ? triangleWrapper.owner : nullptr;
        minInfo.mContact = ray.mP0 + (minInfo.mTI * ray.mV);
	}

    return minInfo;
}

ContactInfo RayTracer::RayCast(const Ray& ray, const Scene& scene, int bounce){

    ContactInfo result;
    if (bounce > mBounces)
    {
        //std::cout << "Early out on bounce number:" << bounce << std::endl;
        return result;
    }

    ContactInfo info = FindClosestObj(ray, scene);
    if(info.IsValid()){

        if (info.mCollidedWith->mbLight) {
            result = info;
            result.mColor = info.mColor;
            return result;
        }
        result = info;
        Ray bounced = info.mCollidedWith->mMaterial->BounceRay(ray.mV, info.mNormal, info.mContact);

        if (bounced.mV == glm::vec3(0.0F)) return result;
		//std::cout << "[RAYCAST]In bounce number: " << bounce << " result color is:" << result.mColor.GetDebugString() << std::endl;
		ContactInfo recursion = RayCast(bounced, scene, bounce + 1);
		//std::cout << "[RAYCAST]After Recursion returned color is:" << recursion.mColor.GetDebugString() << std::endl;
		result.mColor = result.mColor * recursion.mColor;
		//std::cout << "[RAYCAST]After Multiplying result color is:" << result.mColor.GetDebugString() << std::endl;
	}

    return result;
}

void RayTracer::SetBounces(int bounces) { mBounces = bounces; }
int RayTracer::GetBounces() { return mBounces; }

float RayTracer::GetEpsilon() { return mEpsilon; }

#ifdef MULTITHREAD

Ray RayTracer::GetRayForPixel(int x, int y)
{
    glm::vec2 ndc = GraphicsManager.GetNDC({ x,y });
    glm::vec3 pixelworld = GraphicsManager.GetPixelWorld(ndc);
    glm::vec3 camPos = GraphicsManager.GetCameraPos();
    return Ray(camPos, glm::normalize(pixelworld - camPos));
}

#endif // MULTITHREAD