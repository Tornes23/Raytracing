#pragma once
#include <vector>
#include <SFML/System/Vector3.hpp>

class Color
{
public:

	Color(float c = 1.0F);
	Color(float r, float g, float b, float a = 1.0F);
	Color(const std::vector<double>& c);
	Color(const std::vector<float>& c);
	Color(const sf::Vector3<float>& color);
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	static Color White;
	static Color Black;

	static Color Red;
	static Color Green;
	static Color Blue;

	void SetColor(sf::Vector3<float>& color);
	void SetColor(const std::vector<double>& c);
	void SetColor(const std::vector<float>& c);
	sf::Vector3<float> GetColor();

	unsigned char mR;
	unsigned char mG;
	unsigned char mB;
	unsigned char mA;
};