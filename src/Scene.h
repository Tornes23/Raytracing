#pragma once
#include <vector>
#include <string>
#include "Object.h"

class Scene
{
public:
	~Scene();
	Scene(const std::vector<Object>& objects, const std::string& name);

private:

	std::vector<Object> mObjects;

	std::string mName;
};
