#include "Scene.h"

Scene::~Scene()
{
	for (auto& it : mObjects)
	{
		it.Destroy();
	}
}

Scene::Scene(std::vector<Object> objects)
{
	mObjects.resize(objects.size());

	std::memcpy(&mObjects[0], &objects[0], sizeof(Object) * objects.size());
}
