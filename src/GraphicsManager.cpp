#include <glm/gtx/transform.hpp>
#include "GraphicsManager.h"
#include "Utils.h"
#include "Geometry.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Raytracer.h"
#ifdef MULTITHREAD
#include "ThreadPool.h"
#endif // MULTITHREAD


void GraphicsManagerClass::Render() { RenderBatch(0, 0, mWidth, mHeight); mFrameBuffer.Normalize(0, 0, mWidth, mHeight, mSamples); }

void GraphicsManagerClass::RenderBatch(int startX, int startY, int width, int height)
{
	//DEBUG
	//std::cout << "rendering batch = [" << startX << ", " << startY << ", " << width << ", " << height << "]\n";
	//std::cout << "Id of thread executing this thread is = " << std::this_thread::get_id() << "\n";


	for (int x = startX; x < width; x++)
	{
		for (int y = startY; y < height; y++)
		{
			int currScene = SceneManager.GetDisplayScene();
			glm::vec2 ndc = GetNDC({ x,y });
			glm::vec3 pixelworld = GetPixelWorld(ndc);
			glm::vec3 camPos = GetCameraPos(currScene);
			Ray ray(camPos, glm::normalize(pixelworld - camPos));
			Scene* scene = SceneManager.GetScene();
			Color ambient = GetAmbient(currScene);

			ContactInfo info = Raytracer.Cast(ray, scene->mObjects);
			if (info.IsValid())
			{
				if (mRenderNormals)
				{
					Color result(Color((info.mNormal + glm::vec3(1.0F)) / 2.0F) * ambient);
					mFrameBuffer.AddToPixel(x, y, result.mR, result.mG, result.mB);
				}
				else
				{
					//Color result = info.mColor * GetAmbient();
					Color result = info.mColor;
					mFrameBuffer.AddToPixel(x, y, result.mR, result.mG, result.mB);
				}
			}
			else
				mFrameBuffer.AddToPixel(x, y, ambient.mR, ambient.mG, ambient.mB);
		
		}
	}

	//DEBUG
	//std::cout << "Batch = [" << startX << ", " << startY << ", " << width << ", " << height << "] FINISHED! \n";


}


void GraphicsManagerClass::Init(int width, int height)
{
	mFrameBuffer.Init(width, height);
	SetWidth(width);
	SetHeight(height);
	SetAspectRatio((float)width/ height);

	// Generate image and texture to display
	mTexture.create(width, height);
	mImage.create(width, height, sf::Color::Black);
#ifdef MULTITHREAD
	//mBatchSize.x = std::ceil(mWidth / ThreadPool.ThreadCount()) + 1;
	//mBatchSize.y = std::ceil(mHeight / ThreadPool.ThreadCount()) + 1;
#endif // MULTITHREAD

}

void GraphicsManagerClass::ShutDown()
{
	mFrameBuffer.Free();
}

void GraphicsManagerClass::Update()
{
	mFrameBuffer.ConvertFrameBufferToSFMLImage(mImage);
	mTexture.update(mImage);
	mSprite.setTexture(mTexture);
}

void GraphicsManagerClass::Clear() {
#ifdef MULTITHREAD

	int xBatches = mWidth / mBatchSize.x;
	int yBatches = mHeight / mBatchSize.y;

	ThreadPool.SetTaskCount(xBatches * yBatches);

	//for each thread call clear for the wanted coords
	for (int x = 0; x < xBatches; x++)
	{
		for (int y = 0; y < yBatches; y++)
		{
			int startX = x * mBatchSize.x;
			int startY = y * mBatchSize.y;
			ThreadPool.Submit(&FrameBuffer::ClearBatch, &mFrameBuffer, startX, startY, startX + mBatchSize.x, startY + mBatchSize.y, 0, 0, 0);
		}
	}
#else
	mFrameBuffer.Clear();
#endif // MULTITHREAD

}

void GraphicsManagerClass::CreateCamera(const char* info){ mCameras.push_back(Camera(info)); }
void GraphicsManagerClass::CreateLight(const char* info){ mLights.push_back(Light(info));}
void GraphicsManagerClass::ParseAmbient(const char* info){ mAmbientLights.push_back(Color(Utils::GetVector(&info))); }
void GraphicsManagerClass::GetScreenshot(std::string name) { mImage.saveToFile(name); }
void GraphicsManagerClass::AddLight(const Light& light) { mLights.push_back(light); }
glm::vec2 GraphicsManagerClass::GetNDC(const glm::vec2& xy)
{
	float x = ((xy.x + 0.5F) - (mWidth / 2.0F)) / (mWidth / 2.0F);
	float y = -((xy.y + 0.5F) - (mHeight / 2.0F)) / (mHeight / 2.0F);

	return glm::vec2(x, y);
}

glm::vec3 GraphicsManagerClass::GetPixelWorld(const glm::vec2& ndc, bool one_cam)
{
	if (one_cam == false)
	{
		return glm::vec3(0.0F);
	}

	int currScene = SceneManager.GetDisplayScene();
	glm::vec3 pixel = mCameras[currScene].mPos;

	pixel += mCameras[currScene].mFocal * glm::normalize(mCameras[currScene].mTarget - mCameras[currScene].mPos)
		  + ndc.x * (mCameras[currScene].mRight / 2.0F) 
		  + ndc.y * (mCameras[currScene].mUp / (2.0F * mAspectRatio));

	return pixel;
}

glm::vec3 GraphicsManagerClass::GetCameraPos(int index)
{
	if (index < 0)
		return mCameras[0].mPos;
	if (index > mCameras.size())
		return mCameras.back().mPos;

	return mCameras[index].mPos;
}
Camera GraphicsManagerClass::GetCamera(int index)
{
	if (index < 0)
		return mCameras[0];
	if (index > mCameras.size())
		return mCameras.back();

	return mCameras[index];
}
Color GraphicsManagerClass::GetAmbient(int index) 
{ 
	if (index < 0)
		return mAmbientLights[0];
	if (index > mAmbientLights.size())
		return mAmbientLights.back();

	return mAmbientLights[index];
}

const FrameBuffer& GraphicsManagerClass::GetFrameBuffer() { return mFrameBuffer; }
const sf::Image& GraphicsManagerClass::GetImage() { return mImage; }
const sf::Sprite& GraphicsManagerClass::GetSprite() { return mSprite; }
const sf::Texture& GraphicsManagerClass::GetTexture() { return mTexture; }
std::vector<Light>& GraphicsManagerClass::GetLights() { return mLights; }

bool GraphicsManagerClass::RenderNormals(){ return mRenderNormals; }
int GraphicsManagerClass::GetSampleCount() { return mSamples; }
void GraphicsManagerClass::SetWidth(int width) { mWidth = width; }
void GraphicsManagerClass::SetRenderNormals(bool render) { mRenderNormals = render; }
void GraphicsManagerClass::ToggleRenderNormals() { mRenderNormals = !mRenderNormals; }
void GraphicsManagerClass::SetSamples(int count) { mSamples = count; }
void GraphicsManagerClass::SetHeight(int height) { mHeight = height; }
void GraphicsManagerClass::SetAspectRatio(float ratio) { mAspectRatio = ratio;  }

#ifdef MULTITHREAD
#include "ThreadPool.h"
glm::ivec2 GraphicsManagerClass::GetBatchSize() { return mBatchSize; }

void GraphicsManagerClass::BatchedRender()
{
	//int xBatches = 2;
	//int yBatches = 1;
	int xBatches = mWidth / mBatchSize.x;
	int yBatches = mHeight / mBatchSize.y;

	ThreadPool.SetTaskCount(mSamples * xBatches * yBatches);
	//
	//for each thread call render for the wanted coords
	for (int s = 0; s < mSamples; s++) {
	
		for (int x = 0; x < xBatches; x++)
		{
			for (int y = 0; y < yBatches; y++)
			{
				//DEBUG
				//int x = 0;
				//int y = 0;
				int startX = x * mBatchSize.x;
				int startY = y * mBatchSize.y;
				ThreadPool.Submit(&GraphicsManagerClass::RenderBatch, &GetInstance(), startX, startY, startX + mBatchSize.x, startY + mBatchSize.y);
			}
		}
	}

	ThreadPool.SetTaskCount(xBatches * yBatches);
	
	for (int x = 0; x < xBatches; x++)
	{
		for (int y = 0; y < yBatches; y++)
		{
			int startX = x * mBatchSize.x;
			int startY = y * mBatchSize.y;
			ThreadPool.Submit(&FrameBuffer::Normalize, &mFrameBuffer, startX, startY, startX + mBatchSize.x, startY + mBatchSize.y, mSamples);
		}
	}

}
#endif // MULTITHREAD

