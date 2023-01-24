#pragma once
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <glm/vec2.hpp>
#include "Camera.h"
#include "Material.h"

class GraphicsManagerClass
{
public:

	void CreateCamera(const char* info);
	void ParseAmbient(const char* info);
	glm::vec2 GetNDC(const glm::vec2& xy);
	glm::vec3 GetPixelWorld(const glm::vec2& ndc, bool one_cam = true);
	glm::vec3 GetCameraPos(int index = 0);
	Color GetAmbient();
	bool RenderNormals();
	
	void SetWidth(int width);
	void SetHeight(int height);
	void SetAspectRatio(float ratio);
	void SetRenderNormals(bool render);
	void ToggleRenderNormals();

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
	Color mAmbientLight;
	int mWidth = 0;
	int mHeight = 0;
	float mAspectRatio = 0.0F;
	float mRenderNormals = false;

};

#define GraphicsManager (GraphicsManagerClass::GetInstance())
