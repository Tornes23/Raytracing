#include <glm/gtx/transform.hpp>
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "Raytracer.h"
#include "Object.h"
#include "Utils.h"

ContactInfo RayTracer::Cast(const Ray& ray, std::vector<Object>& objs)
{
    return RayCast(ray, objs, 1);
}

ContactInfo RayTracer::FindClosestObj(const Ray& ray, std::vector<Object>& objs)
{
    ContactInfo minInfo;
    minInfo.mTI = std::numeric_limits<float>::max();
    int minIndex = -1;

    for (int i = 0; i < objs.size(); i++)
    {
        ContactInfo info;
        bool intersected = objs[i].CheckIntersection(ray, info);

        if (intersected)
        {
            if (info.mTI < minInfo.mTI){
                minInfo = info;
                minIndex = i;
            }
        }
    }

    if (minIndex >= 0 && objs[minIndex].mbLight) {
        minInfo.mColor = objs[minIndex].mMaterial->mColor;
    }

    if(minIndex >= 0)
        minInfo.mCollidedWith = &objs[minIndex];


    return minInfo;
}

ContactInfo RayTracer::RayCast(const Ray& ray, std::vector<Object>& objs, int bounce){

    ContactInfo result;
    if (bounce > mBounces)
    {
        //std::cout << "Early out on bounce number:" << bounce << std::endl;
        return result;
    }

    ContactInfo info = FindClosestObj(ray, objs);
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
		ContactInfo recursion = RayCast(bounced, objs, bounce + 1);
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