#pragma once
#include <string>
#include <glm/vec3.hpp>

namespace Utils
{
	glm::vec3 GetVector(const char** info);
	float GetFloat(const char** info);
	std::string GetFilename(const std::string& path);
	void LoadConfig(const std::string& path);

}