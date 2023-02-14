#pragma once
#include <iostream>
#include <future>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <glm/vec2.hpp>
#include <glm/vec2.hpp>
#include <SFML/Graphics.hpp>

#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "FrameBuffer.h"

class GraphicsManagerClass
{
public:
	void Init(int width, int height);
	void Update();

	void CreateCamera(const char* info);
	void CreateLight(const char* info);
	void ParseAmbient(const char* info);
	void GetScreenshot(std::string name);

	glm::vec2 GetNDC(const glm::vec2& xy);
	glm::vec3 GetPixelWorld(const glm::vec2& ndc, bool one_cam = true);
	glm::vec3 GetCameraPos(int index = 0);
	Camera GetCamera(int index = 0);
	Color GetAmbient(int index = 0);
	sf::Image& GetImage();
	sf::Sprite& GetSprite();
	sf::Texture& GetTexture();

	bool RenderNormals();

	void SetWidth(int width);
	void SetHeight(int height);
	void SetAspectRatio(float ratio);
	void SetRenderNormals(bool render);
	void ToggleRenderNormals();

	void Render();
	void RenderBatch(int startX, int startY, int width, int height);



#ifdef MULTITHREAD
	glm::ivec2 GetBatchSize();
	void BatchedRender();
#endif // MULTITHREAD


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
	std::vector<Light> mLights;
	std::map<std::string, std::shared_ptr<Material>> mMaterials;
	std::vector<Color> mAmbientLights;
	int mWidth = 0;
	int mHeight = 0;
	float mAspectRatio = 0.0F;
	float mRenderNormals = true;
	FrameBuffer mFrameBuffer;

	sf::Image   mImage;
	sf::Texture mTexture;
	sf::Sprite  mSprite;

#ifdef MULTITHREAD
	glm::ivec2 mBatchSize{250,250};
#endif // MULTITHREAD

};

#define GraphicsManager (GraphicsManagerClass::GetInstance())
