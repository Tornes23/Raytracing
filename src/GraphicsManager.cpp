#include <glm/gtx/transform.hpp>
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

glm::vec2 GraphicsManagerClass::GetNDC(const glm::vec2& xy)
{
	float x = ((xy.x + 0.5F) - (mWidth / 2.0F)) / (mWidth / 2.0F);
	float y = -((xy.y + 0.5F) - (mHeight / 2.0F)) / (mHeight / 2.0F);

	return glm::vec2(x, y);
}

glm::vec3 GraphicsManagerClass::GetPixelWorld(const glm::vec2& ndc, bool one_cam)
{
	if (one_cam == false)
	{
		return glm::vec3(0.0F);
	}

	glm::vec3 pixel = mCameras[0].mPos;

	pixel += mCameras[0].mFocal * glm::normalize(mCameras[0].mPos - mCameras[0].mTarget)
		  + ndc.x * (mCameras[0].mRight / 2.0F) 
		  + ndc.y * (mCameras[0].mUp / (2.0F * mAspectRatio));

	return pixel;
}

glm::vec3 GraphicsManagerClass::GetCameraPos(int index)
{
	if (index < 0)
		return mCameras[0].mPos;
	if (index > mCameras.size())
		return mCameras.back().mPos;

	return mCameras[index].mPos;
}

void GraphicsManagerClass::SetWidth(int width) { mWidth = width; }
void GraphicsManagerClass::SetHeight(int height) { mHeight = height; }
void GraphicsManagerClass::SetAspectRatio(float ratio) { mAspectRatio = ratio;  }
