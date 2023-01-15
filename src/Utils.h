#pragma once
#include <SFML/System/Vector3.hpp>
namespace Utils
{
	void GetData(const char* info);
	sf::Vector3<float> GetVector(const char* info, size_t& start);
	float GetFloat(const char* info, int start, int end);

}