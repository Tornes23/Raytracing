#include <iostream>
#include "Scene.h"

Scene::~Scene()
{
	for (int i = 0; i < mObjects.size(); i++)
		mObjects[i].Destroy();

	mObjects.clear();
}

Scene::Scene(const std::vector<Object>& objects, const std::string& name)
{
	mName = name;

	mObjects.resize(objects.size());

	for (int i = 0; i < objects.size(); i++)
		mObjects[i] = objects[i];
}
