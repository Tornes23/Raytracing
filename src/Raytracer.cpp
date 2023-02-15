#include <glm/gtx/transform.hpp>
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "Raytracer.h"
#include "Utils.h"

ContactInfo RayTracer::CastRay(const Ray& ray, std::vector<Object>& objs)
{
    ContactInfo result = RayPath(ray, objs, 0);
 
    return result;
}

ContactInfo RayTracer::RayPath(const Ray& ray, std::vector<Object>& objs, int _bounce)
{
    ContactInfo info;
    if (_bounce > mBounces) {
        return info;
    }
    ContactInfo minInfo;
    minInfo.mT0 = std::numeric_limits<float>::max();

    for (int i = 0; i < objs.size(); i++)
    {
        bool intersected = objs[i].CheckIntersection(ray, info);

        if (intersected)
        {
            if (info.mTI < minInfo.mTI)
                minInfo = info;

            if (objs[i].mbLight)
            {
                minInfo.mColor = minInfo.mColor * objs[i].mMaterial.mDiffuse;
                return minInfo;
            }

            Ray bounce(info.mContact + mEpsilon, info.mNormal + Utils::GetRandomVector());
            ContactInfo recur = RayPath(bounce, objs, _bounce + 1);
        }
    }

    return minInfo;
}


void RayTracer::SetBounces(int bounces) { mBounces = bounces; }
void RayTracer::SetSamples(int samples) { mSamples = samples; }

int RayTracer::GetBounces() { return mBounces; }
int RayTracer::GetSamples() { return mSamples; }

#ifdef MULTITHREAD

Ray RayTracer::GetRayForPixel(int x, int y)
{
    glm::vec2 ndc = GraphicsManager.GetNDC({ x,y });
    glm::vec3 pixelworld = GraphicsManager.GetPixelWorld(ndc);
    glm::vec3 camPos = GraphicsManager.GetCameraPos();
    return Ray(camPos, glm::normalize(pixelworld - camPos));
}

#endif // MULTITHREAD