#pragma once
#include <fstream>
#include <string>
#include "Color.h"

struct Material
{
	Material(Color diffuse = Color::White) : mDiffuse(diffuse) {}
	void ParseData(std::ifstream& file);

	Color mDiffuse;
};