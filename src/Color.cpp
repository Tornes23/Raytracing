#include <glm/glm.hpp>
#include <string>
#include "Color.h"

//setting the static variables
Color Color::White = Color(1.0F, 1.0F, 1.0F);
Color Color::Black = Color(0.0F, 0.0F, 0.0F);
Color Color::Red = Color(1.0F, 0.0F, 0.0F);
Color Color::Green = Color(0.0F, 1.0F, 0.0F);
Color Color::Blue = Color(0.0F, 0.0F, 1.0F);

Color::Color(float r, float g, float b)
{
	//setting the values
	mRGB.r = r;
	mRGB.g = g;
	mRGB.b = b;
}

Color::Color(float c)
{
	mRGB.r = c;
	mRGB.g = c;
	mRGB.b = c;
}

Color::Color(const std::vector<double>& c)
{
	//setting the values
	mRGB.r = (float)c[0];
	mRGB.g = (float)c[1];
	mRGB.b = (float)c[2];
}

Color::Color(const std::vector<float>& c)
{
	//setting the values
	mRGB.r = c[0];
	mRGB.g = c[1];
	mRGB.b = c[2];
}

Color::Color(const glm::vec3& color)
{
	mRGB = color;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b)
{
	//setting the values
	mRGB.r = r / 255.0F;
	mRGB.g = g / 255.0F;
	mRGB.b = b / 255.0F;
}

Color::Color(const Color& color) { mRGB = color.GetColor(); }

void Color::SetColor(glm::vec3& color)
{
	mRGB = color;
}

void Color::SetColor(const std::vector<double>& c)
{
	mRGB.r = static_cast<float>(c[0]);
	mRGB.g = static_cast<float>(c[1]);
	mRGB.b = static_cast<float>(c[2]);
}

void Color::SetColor(const std::vector<float>& c)
{
	mRGB.r = c[0];
	mRGB.g = c[1];
	mRGB.b = c[2];
}

glm::vec3 Color::GetColor() const{ return mRGB; }
glm::vec<3, unsigned char> Color::ToRGB(int divisor) const { 

	unsigned char r = (unsigned char)glm::clamp<float>(glm::ceil((mRGB.r / divisor) * 255), 0, 255);
	unsigned char g = (unsigned char)glm::clamp<float>(glm::ceil((mRGB.g / divisor) * 255), 0, 255);
	unsigned char b = (unsigned char)glm::clamp<float>(glm::ceil((mRGB.b / divisor) * 255), 0, 255);

	return glm::vec<3, unsigned char>(r, g, b); 
}
float Color::GetR() const { return mRGB.r; }
float Color::GetG() const { return mRGB.g; }
float Color::GetB() const { return mRGB.b; }

Color Color::operator*(const Color& rhs)
{
	glm::vec3 floats = GetColor();
	glm::vec3 rhsFloats = rhs.GetColor();
	return Color(floats * rhsFloats);
}

std::string Color::GetDebugString() const
{
	std::string debugString("(");
	debugString += std::to_string(mRGB.r);
	debugString += ",";
	debugString += std::to_string(mRGB.g);
	debugString += ",";
	debugString += std::to_string(mRGB.b);
	debugString += ")";
	return debugString;
}






