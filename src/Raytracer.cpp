#include <glm/gtx/transform.hpp>
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "Raytracer.h"
#include "Object.h"
#include "Utils.h"

ContactInfo RayTracer::Cast(const Ray& ray, std::vector<Object>& objs)
{
    ContactInfo result;

    ContactInfo info = FindClosestObj(ray, objs);

    if (info.IsValid()) {
        
        if (info.mCollidedWith->mbLight) {
            result.mTI = 1.0F;
            result.mColor = info.mColor;
            return result;
        }

        result = info;
        Ray bounce = info.mCollidedWith->mMaterial->BounceRay(ray.mV, info.mNormal, info.mContact);
        if (bounce.mV == glm::vec3(0.0F)) return result;
        result.mColor = result.mColor * RayCast(bounce, objs, 1).mColor;
        auto test = result.mColor.ToRGB(GraphicsManager.GetSampleCount());
    }
            
    return result;
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
    if (bounce > mBounces) return result;

    ContactInfo info = FindClosestObj(ray, objs);
    if(info.IsValid()){

        if (info.mCollidedWith->mbLight) {
            result.mTI = 1.0F;
            result.mColor = info.mColor;
            return result;
        }
        result = info;
        Ray bounced = info.mCollidedWith->mMaterial->BounceRay(ray.mV, info.mNormal, info.mContact);

        if (bounced.mV == glm::vec3(0.0F)) return result;

        result.mColor = result.mColor * RayCast(bounced, objs, bounce + 1).mColor;
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