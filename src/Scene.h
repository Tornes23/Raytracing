#pragma once
#include <vector>
#include <string>
#include "Object.h"
#include "KDTree.hpp"

struct Scene
{
	~Scene();
	Scene(const std::vector<Object>& objects, const std::string& name);
	Scene(const std::string& name = "InvalidSceneName");

	void SetObjects(std::vector<Object>& objects);
	void SubmitTrianglesOfObject(const Object& object);
	void SubmitTrianglesOfObjects();
	void BuildSceneKDTree();
	
	void SetKDTreeTraversalCost(float cost);
	void SetKDTreeIntersectionCost(float cost);
	void SetKDTreeMaxDepth(int maxDepth);

	std::vector<Object> mObjects;
	std::vector<scene_triangle> mSceneTriangles;

	kdtree mKDTree;

	kdtree::config mKDTreeConfig;
	std::string mName;
};
