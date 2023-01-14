#pragma once
#include <vector>
#include <string>
#include "Object.h"

class Scene
{
public:
	~Scene();

private:

	std::vector<Object> mObjects;

	std::string mName;
};
