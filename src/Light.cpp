#include "Utils.h"
#include "Light.h"

Light::Light(const char* info)
{
	if (info == nullptr)
		return;

	mPos = Utils::GetVector(&info);
	mRadius = Utils::GetFloat(&info);
	mIntensity= Utils::GetVector(&info);
}
