#include "Raytracer.h"

ContactInfo RayTracer::CastRay(const Ray& ray, std::vector<Object>& objs)
{
    ContactInfo info;
    ContactInfo minInfo;
    minInfo.mTI = std::numeric_limits<float>::max();

    for (int i = 0; i < objs.size(); i++)
    {
        bool intersected = objs[i].CheckIntersection(ray, info);
        
        if(intersected)
        {
            if (info.mTI < minInfo.mTI)
                minInfo = info;
        }
    }

    return minInfo;
}

void RayTracer::SetBounces(int bounces) { mBounces = bounces; }
void RayTracer::SetSamples(int samples) { mSamples = samples; }

int RayTracer::GetBounces() { return mBounces; }
int RayTracer::GetSamples() { return mSamples; }
