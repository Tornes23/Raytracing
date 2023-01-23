#pragma once
#include <glm/vec3.hpp>
#include <string>

class Camera
{
public:
	Camera(const char* info = nullptr);

private:

	glm::vec3 mPos{0.0F, 0.0F, 0.0F };
	glm::vec3 mTarget{0.0F, 0.0F, -1.0F };
	glm::vec3 mUp{0.0F, 1.0F, 0.0F };

	float mFocal;
	std::string mName;
};