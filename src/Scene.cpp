#include <iostream>
#include "Scene.h"

Scene::~Scene()
{
	for (int i = 0; i < mObjects.size(); i++)
		mObjects[i].Destroy();

	mObjects.clear();
	mSceneTriangles.clear();
}

Scene::Scene(const std::vector<Object>& objects, const std::string& name)
{
	mName = name;

	mObjects.resize(objects.size());

	for (int i = 0; i < objects.size(); i++)
		mObjects[i] = objects[i];
}

Scene::Scene(const std::string& name)
{
	mName = name;
}

void Scene::SetObjects(std::vector<Object>& objects)
{
	mObjects.resize(objects.size());

	std::memcpy(&mObjects[0], &objects[0], sizeof(Object) * objects.size());
}

void Scene::SubmitTrianglesOfObject(Object* object)
{
	std::vector<Triangle>& meshTriangles = object->mGeometry->mModel->mTriangles;
	if (meshTriangles.empty())
	{
		return;
	}

	std::vector<scene_triangle> oldTriangles = mSceneTriangles;
	std::vector<scene_triangle> newTriangles(meshTriangles.size());
	//push back triangles with object* data and geometry
	for (int i = 0; i < newTriangles.size(); i++)
	{
		newTriangles[i].geometry = meshTriangles[i];
		newTriangles[i].owner = object;
	}

	mSceneTriangles.resize(oldTriangles.size() + newTriangles.size());
	if (!oldTriangles.empty())
	{
		std::memcpy(&mSceneTriangles[0], &oldTriangles[0], sizeof(scene_triangle) * oldTriangles.size());
	}

	std::memcpy(&mSceneTriangles[oldTriangles.size()], &newTriangles[0], sizeof(scene_triangle) * newTriangles.size());
}

void Scene::SubmitTrianglesOfObjects()
{
	for (size_t i = 0; i < mObjects.size(); i++)
	{
		if (mObjects[i].mGeometryType != GeometryTypes::Model)
		{
			continue;
		}

		SubmitTrianglesOfObject(&mObjects[i]);
	}
}

void Scene::BuildSceneKDTree()
{
	if (mSceneTriangles.empty())
	{
		return;
	}

	mKDTree.build(mSceneTriangles, mKDTreeConfig);
}

void Scene::SetKDTreeTraversalCost(float cost)
{
	mKDTreeConfig.cost_traversal = cost;
}

void Scene::SetKDTreeIntersectionCost(float cost)
{
	mKDTreeConfig.cost_intersection = cost;
}

void Scene::SetKDTreeMaxDepth(int maxDepth)
{
	mKDTreeConfig.max_depth = maxDepth;
}

void Scene::SetKDTreeMinTriangles(int minTriangles)
{
	mKDTreeConfig.min_triangles = minTriangles;
}

