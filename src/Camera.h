#pragma once
#include <glm/vec3.hpp>
#include <string>

struct Camera
{
	Camera(const char* info = nullptr);

	glm::vec3 mPos{0.0F, 0.0F, 0.0F };
	glm::vec3 mTarget{0.0F, 0.0F, -1.0F };
	glm::vec3 mUp{0.0F, 1.0F, 0.0F };
	glm::vec3 mRight{1.0F, 0.0F, 0.0F };

	float mFocal = 1.0F;
	std::string mName;
};