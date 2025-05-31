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
	void Init();
	void ShutDown();
	void Update();
	void Clear();
	void Normalize();
	void UpdateTextures();

	void CreateCamera(const char* info);
	void CreateLight(const char* info, const std::string& scene);
	void ParseAmbient(const char* info);
	void GetScreenshot(std::string name);
	void AddLight(const Light& light, const std::string& scene);
	void IncrementSampleCount();

	glm::vec2 GetNDC(const glm::vec2& xy);
	glm::vec2 GetRandomSubPixelNDC(const glm::vec2& xy);
	glm::vec3 GetPixelWorld(const glm::vec2& ndc, bool one_cam = true);
	glm::vec3 GetCameraPos(int index = 0);
	Camera GetCamera(int index = 0);
	Color GetAmbient(int index = 0);
	const sf::Image& GetImage();
	const sf::Sprite& GetSprite();
	const sf::Texture& GetTexture();
	const FrameBuffer& GetFrameBuffer();
	std::vector<Light>& GetLights(const std::string& key);
	glm::ivec2 GetSize();

	bool SwapBuffers();
	bool RenderNormals();
	int GetTotalSamples();
	int GetSampleCount();
	bool HasFinished() const;

	void SetWidth(int width);
	void SetHeight(int height);
	void SetAspectRatio(float ratio);
	void SetAspectRatio();
	void SetRenderNormals(bool render);
	void ToggleRenderNormals();
	void SetSamples(int count = 1);
	void SetAntiAliasingSamples(int count = 1);

	void Render();
	void RenderBatch(int startX, int startY, int endX, int endY);



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
	std::map<std::string , std::vector<Light>> mLights;
	std::map<std::string, std::shared_ptr<Material>> mMaterials;
	std::vector<Color> mAmbientLights;
	int mWidth = 0;
	int mHeight = 0;
	float mAspectRatio = 0.0F;
	bool mRenderNormals = false;
	//float mRenderNormals = true;
	FrameBuffer mFrameBuffer;

	sf::Image   mImage;
	sf::Texture mTexture;
	sf::Sprite  mSprite;
	int  mSamples = 10;
	int  mSampleCount = 0;
	int  mMSAASamples = 0;
	bool  mbNormalized = false;
	bool  mbSampleIncremented = false;

#ifdef MULTITHREAD
	glm::ivec2 mBatchSize{50,50};
#endif // MULTITHREAD

};

#define GraphicsManager (GraphicsManagerClass::GetInstance())
