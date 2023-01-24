#include <glm/gtx/transform.hpp>
#include "Utils.h"
#include "Camera.h"

Camera::Camera(const char* info)
{
	if (info == nullptr)
		return;

	//get the vectors and stuff
	mPos = Utils::GetVector(&info);
	mTarget = Utils::GetVector(&info);
	mUp = Utils::GetVector(&info);
	mFocal = Utils::GetFloat(&info);
	glm::vec3 right = glm::cross(mUp, (mTarget - mPos));
	mUp = glm::normalize(glm::cross((mTarget - mPos), right));
	mRight = glm::normalize(glm::cross((mTarget - mPos), mUp));
}
