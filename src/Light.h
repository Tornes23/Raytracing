#pragma once
#include <glm/vec3.hpp>
struct Light
{
	Light(const char* info = nullptr);
	glm::vec3 mPos{ 0.0F, 0.0F, 0.0F };
	float mRadius = 1.0F;
	glm::vec3 mColor{ 1.0F, 1.0F, 1.0F };
};