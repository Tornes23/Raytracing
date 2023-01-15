#pragma once
#include <fstream>
#include <string>
#include "Color.h"

struct Material
{
	Material(Color diffuse = Color::White, std::string name = "Default") : mDiffuse(diffuse), mName(name) {}
	void ParseData(std::ifstream& file);

	Color mDiffuse;
	std::string mName;
};