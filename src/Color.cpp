#include "Color.h"

//setting the static variables
Color Color::White = Color(1.0F, 1.0F, 1.0F, 1.0F);
Color Color::Black = Color(0.0F, 0.0F, 0.0F, 1.0F);
Color Color::Red = Color(1.0F, 0.0F, 0.0F, 1.0F);
Color Color::Green = Color(0.0F, 1.0F, 0.0F, 1.0F);
Color Color::Blue = Color(0.0F, 0.0F, 1.0F, 1.0F);

Color::Color(float r, float g, float b, float a)
{
	//setting the values
	mR = static_cast<unsigned char>(r * 255);
	mG = static_cast<unsigned char>(g * 255);
	mB = static_cast<unsigned char>(b * 255);
	mA = static_cast<unsigned char>(a * 255);
}

Color::Color(float c)
{
	mR = static_cast<unsigned char>(c * 255);
	mG = static_cast<unsigned char>(c * 255);
	mB = static_cast<unsigned char>(c * 255);
	mA = static_cast <unsigned char>(255);
}

Color::Color(const std::vector<double>& c)
{
	//setting the values
	mR = static_cast<unsigned char>(c[0] * 255);
	mG = static_cast<unsigned char>(c[1] * 255);
	mB = static_cast<unsigned char>(c[2] * 255);
	mA = static_cast<unsigned char>(c[3] * 255);
}

Color::Color(const std::vector<float>& c)
{
	//setting the values
	mR = static_cast<unsigned char>(c[0] * 255);
	mG = static_cast<unsigned char>(c[1] * 255);
	mB = static_cast<unsigned char>(c[2] * 255);
	mA = static_cast<unsigned char>(c[3] * 255);
}

Color::Color(const glm::vec3& color)
{
	mR = static_cast<unsigned char>(color.x * 255);
	mG = static_cast<unsigned char>(color.y * 255);
	mB = static_cast<unsigned char>(color.z * 255);
	mA = 255;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	//setting the values
	mR = r;
	mG = g;
	mB = b;
	mA = a;
}

void Color::SetColor(glm::vec3& color)
{
	mR = static_cast<unsigned char>(color.x * 255);
	mG = static_cast<unsigned char>(color.y * 255);
	mB = static_cast<unsigned char>(color.z * 255);
	mA = 255;
}

void Color::SetColor(const std::vector<double>& c)
{
	mR = static_cast<unsigned char>(c[0] * 255);
	mG = static_cast<unsigned char>(c[1] * 255);
	mB = static_cast<unsigned char>(c[2] * 255);
	mA = static_cast<unsigned char>(c[3] * 255);
}

void Color::SetColor(const std::vector<float>& c)
{
	mR = static_cast<unsigned char>(c[0] * 255);
	mG = static_cast<unsigned char>(c[1] * 255);
	mB = static_cast<unsigned char>(c[2] * 255);
	mA = static_cast<unsigned char>(c[3] * 255);
}

glm::vec3 Color::GetColor()
{
	float r = static_cast<float>(mR) / 255.0F;
	float g = static_cast<float>(mG) / 255.0F;
	float b = static_cast<float>(mB) / 255.0F;

	return glm::vec3(r, g, b);
}

Color Color::operator*(const Color& rhs)
{
	return Color(mR * rhs.mR, mG * rhs.mG, mB * rhs.mB);
}




