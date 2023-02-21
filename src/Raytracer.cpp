#include <glm/gtx/transform.hpp>
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "Raytracer.h"
#include "Utils.h"

ContactInfo RayTracer::Cast(const Ray& ray, std::vector<Object>& objs)
{
    ContactInfo result;

    ContactInfo info = RayCast(ray, objs);

    if (info.mbWithLight) {
        result.mTI = 1.0F;
        result.mColor = info.mColor;
        return result;
    }

    if (info.IsValid()) {
        result = info;
        result.mColor = BounceRay(ComputeBounceRay(info.mNormal, info.mContact), objs, 1).mColor;
    }
            
    return result;
}

ContactInfo RayTracer::RayCast(const Ray& ray, std::vector<Object>& objs)
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
        minInfo.mColor = objs[minIndex].mMaterial.mDiffuse;
        minInfo.mbWithLight = true;
    }

    return minInfo;
}

ContactInfo RayTracer::BounceRay(const Ray& ray, std::vector<Object>& objs, int bounce){

    ContactInfo result;
    if (bounce > mBounces) return result;

    ContactInfo info = RayCast(ray, objs);

    if (info.mbWithLight) {
        result.mTI = 1.0F;
        result.mColor = result.mColor * info.mColor;
        return result;
    }

    if (info.IsValid()) {
        result = info;
        result.mColor = result.mColor * BounceRay(ComputeBounceRay(info.mNormal, info.mContact), objs, bounce + 1).mColor;
    }

    return result;
}

Ray RayTracer::ComputeBounceRay(const glm::vec3& normal, const glm::vec3& contact) { return Ray(contact + mEpsilon, normal + Utils::GetRandomVector()); }
void RayTracer::SetBounces(int bounces) { mBounces = bounces; }
int RayTracer::GetBounces() { return mBounces; }

#ifdef MULTITHREAD

Ray RayTracer::GetRayForPixel(int x, int y)
{
    glm::vec2 ndc = GraphicsManager.GetNDC({ x,y });
    glm::vec3 pixelworld = GraphicsManager.GetPixelWorld(ndc);
    glm::vec3 camPos = GraphicsManager.GetCameraPos();
    return Ray(camPos, glm::normalize(pixelworld - camPos));
}

#endif // MULTITHREAD