#pragma once
#include <SFML/System/Vector3.hpp>
#include <string>

struct Material
{
	sf::Vector3<float> mDiffuse;
	std::string mName;
};