#pragma once
#include <fstream>
#include <string>
#include <glm/vec3.hpp>
struct Material;

namespace Utils
{
	glm::vec3 GetVector(const char** info);
	float GetFloat(const char** info);
	int GetInt (const char** info);
	glm::vec3 GetVector(char** info);
	float GetFloat(char** info);
	int GetInt (char** info);
	int GetInt (const char* info);
	std::string GetFile(const char* info);
	Material* ParseMaterial(std::ifstream& file);

	std::string GetFilename(const std::string& path);
	void LoadConfig(const std::string& path);
	glm::vec3 GetRandomVector();
	glm::vec3 GetRandomVector(float radius);

}