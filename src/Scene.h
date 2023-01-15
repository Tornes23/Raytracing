#pragma once
#include <vector>
#include <string>
#include "Object.h"

class Scene
{
public:
	~Scene();
	Scene(std::vector<Object> objects);

private:

	std::vector<Object> mObjects;

	std::string mName;
};
