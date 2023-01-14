#include "Scene.h"

Scene::~Scene()
{
	for (auto& it : mObjects)
	{
		it.Destroy();
	}
}
