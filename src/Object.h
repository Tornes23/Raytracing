#pragma once
#include <memory>
#include <string>
#include <SFML/System/Vector3.hpp>

struct Geometry;
struct Object
{
	void Destroy();
	sf::Vector3<float> mPos{0.0F, 0.0F, 0.0F};
	sf::Vector3<float> mForward{ 0.0F, 0.0F, 0.0F };
	sf::Vector3<float> mRight{ 0.0F, 0.0F, 0.0F };

	//the corresponding Geometry
	std::shared_ptr<Geometry> mModel;
};