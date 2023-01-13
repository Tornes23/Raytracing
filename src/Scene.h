#pragma once
#include <string>
#include "Object.h"

class Scene
{
public:
	Scene();
	~Scene();

private:

	std::vector<Object> mObjects;

	std::string mName;
};
