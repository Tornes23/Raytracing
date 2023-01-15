#pragma once
#include <SFML/System/Vector3.hpp>
#include <string>

class Camera
{
public:
	Camera(const char* info = nullptr);

private:

	sf::Vector3<float> mPos{0.0F, 0.0F, 0.0F };
	sf::Vector3<float> mTarget{0.0F, 0.0F, -1.0F };
	sf::Vector3<float> mUp{0.0F, 1.0F, 0.0F };

	float mFocal;
	std::string mName;
};