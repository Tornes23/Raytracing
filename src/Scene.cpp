#include "Scene.h"

Scene::~Scene()
{
	for (auto& it : mObjects)
		it.Destroy();
}

Scene::Scene(std::vector<Object> objects)
{
	mObjects.resize(objects.size());

	for (int i = 0; i < objects.size(); i++)
		mObjects[i] = objects[i];
}
