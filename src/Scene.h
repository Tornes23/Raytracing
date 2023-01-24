#pragma once
#include <vector>
#include <string>
#include "Object.h"

struct Scene
{
	~Scene();
	Scene(const std::vector<Object>& objects, const std::string& name);
	
	std::vector<Object> mObjects;

	std::string mName;
};
