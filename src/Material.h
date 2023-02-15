#pragma once
#include <fstream>
#include <string>
#include "Color.h"

struct Material
{
	Material(Color diffuse = Color::White) : mDiffuse(diffuse), mRoughness(1.0F){}
	void ParseData(std::ifstream& file);

	Color mDiffuse;
	float mRoughness;
};