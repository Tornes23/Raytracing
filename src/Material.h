#pragma once
#include <string>
#include "Color.h"
#include "Raytracer.h"


struct Material
{
	Material(Color color = Color::White) : mColor(color) {}
	virtual void ParseData(const char** info) = 0;
	virtual Ray BounceRay(const glm::vec3& normal, const glm::vec3& cp) = 0;
	Color mColor;
};

struct Diffuse : Material
{
	Diffuse(){}
	void ParseData(const char** info);
	Ray BounceRay(const glm::vec3& normal, const glm::vec3& cp);
};

struct Metal : Material
{
	Metal(){}
	void ParseData(const char** info);
	Ray BounceRay(const glm::vec3& normal, const glm::vec3& cp);
	float mRoughness = 0.0F;
};

struct Dielectric : Material
{
	Dielectric() {}
	void ParseData(const char** info);
	Ray BounceRay(const glm::vec3& normal, const glm::vec3& cp);
	Color mTraversal;
};