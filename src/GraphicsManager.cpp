#include "GraphicsManager.h"
#include "Utils.h"

void GraphicsManagerClass::CreateCamera(const char* info)
{
	mCameras.push_back(Camera(info));
}

void GraphicsManagerClass::GetAmbient(const char* info)
{
	mAmbientLight = Color(Utils::GetVector(&info));
}
