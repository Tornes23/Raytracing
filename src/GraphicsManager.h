#pragma once
#include <memory>
#include <vector>
#include <string>
#include <map>
#include "Camera.h"
#include "Material.h"

class GraphicsManagerClass
{
public:

	void CreateCamera(const char* info);

	//singleton stuff
	GraphicsManagerClass(GraphicsManagerClass const&) = delete;
	void operator=(GraphicsManagerClass const&) = delete;
	static GraphicsManagerClass& GetInstance()
	{
		static GraphicsManagerClass instance;
		return instance;
	}

private:
	GraphicsManagerClass() {}

	std::vector<Camera> mCameras;
	std::map<std::string, std::shared_ptr<Material>> mMaterials;

};

#define GraphicsManager (GraphicsManagerClass::GetInstance())
