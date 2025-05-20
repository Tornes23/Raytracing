#include <glm/gtx/transform.hpp>
#include "GraphicsManager.h"
#include "Utils.h"
#include "Geometry.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Raytracer.h"

#ifdef MULTITHREAD
#include <Windows.h>
#include "ThreadPool.h"
#endif // MULTITHREAD


//void GraphicsManagerClass::Render() { if (!SwapBuffers()) return; RenderBatch(40, 150, 41, 151); }
void GraphicsManagerClass::Render() { if (!SwapBuffers()) return; RenderBatch(0, 0, mWidth, mHeight); }

void GraphicsManagerClass::RenderBatch(int startX, int startY, int endX, int endY)
{
	//DEBUG
	//std::cout << "rendering batch = [" << startX << ", " << startY << ", " << width << ", " << height << "]\n";
	//std::cout << "Id of thread executing this thread is = " << std::this_thread::get_id() << "\n";
	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
		{
			int currScene = SceneManager.GetDisplayScene();
			glm::vec2 ndc = GetNDC({ x,y });
			glm::vec3 pixelworld = GetPixelWorld(ndc);
			glm::vec3 camPos = GetCameraPos(currScene);
			Ray ray(camPos, glm::normalize(pixelworld - camPos));
			Scene* scene = SceneManager.GetScene();
			Color ambient = GetAmbient(currScene);

			ContactInfo info = Raytracer.RayCast(ray, scene->mObjects, 0);

			if (mMSAASamples > 0)
			{

				for (int i = 0; i < mMSAASamples; i++)
				{
					// Generating a random subpixel sample coords
					glm::vec2 randNDC = GetRandomSubPixelNDC({ x,y });
					// the world position of the pixel with the random subsampling
					glm::vec3 subSamplePixelworld = GetPixelWorld(randNDC);
					// the ray will go through that position in the world
					Ray subSampleRay(camPos, glm::normalize(subSamplePixelworld - camPos));
					//make the anti aliasing sample
					ContactInfo iterationResult = Raytracer.RayCast(subSampleRay, scene->mObjects, 0);
					info.mColor += iterationResult.mColor;
				}

				info.mColor /= mMSAASamples;
			}

			Color pixelColor = mFrameBuffer.GetColorFromPixel(x,y);
			Color rayColor = info.IsValid() ? info.mColor : ambient;

			if (mRenderNormals)
			{
				rayColor = Color((info.mNormal + glm::vec3(1.0F)) / 2.0F);
			}

			// mix the color
			Color addedColor = pixelColor * rayColor;
	
			mFrameBuffer.AddToPixel(x, y, rayColor);
		}
	}
	//DEBUG
	//std::cout << "Batch = [" << startX << ", " << startY << ", " << width << ", " << height << "] FINISHED! \n";
}


void GraphicsManagerClass::Init()
{
	mFrameBuffer.Init(mWidth, mHeight);
	SetAspectRatio();

	// Generate image and texture to display
	mTexture.create(mWidth, mHeight);
	mImage.create(mWidth, mHeight, sf::Color::Black);
}

void GraphicsManagerClass::ShutDown()
{
	mFrameBuffer.Free();
}

void GraphicsManagerClass::Update()
{
	if (GraphicsManager.SwapBuffers())
		IncrementSampleCount();
}

void GraphicsManagerClass::Clear() {
#ifdef MULTITHREAD

	if (!ThreadPool.HasFinished()) return;

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
			ThreadPool.Submit(&FrameBuffer::ClearBatch, &mFrameBuffer, startX, startY, startX + mBatchSize.x, startY + mBatchSize.y, glm::vec3(0.0F));
		}
	}
	mSampleCount = 0;
#else
	mFrameBuffer.Clear();
	mSampleCount = 0;
#endif // MULTITHREAD

}

void GraphicsManagerClass::Normalize()
{
#ifdef MULTITHREAD

	if (!mbNormalized) {
		if (!ThreadPool.HasFinished()) return;
		mbNormalized = true;
		int xBatches = mWidth / mBatchSize.x;
		int yBatches = mHeight / mBatchSize.y;
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
#else
	if (!mbNormalized) {
		mbNormalized = true;
		mFrameBuffer.Normalize(0, 0, mWidth, mHeight, mSamples);
	}
#endif // MULTITHREAD
}

void GraphicsManagerClass::UpdateTextures()
{
	mFrameBuffer.ConvertFrameBufferToSFMLImage(mImage, mSampleCount);
	mTexture.update(mImage);
	mSprite.setTexture(mTexture);
}

void GraphicsManagerClass::CreateCamera(const char* info){ mCameras.push_back(Camera(info)); }
void GraphicsManagerClass::CreateLight(const char* info, const std::string& scene){ mLights[scene].push_back(Light(info));}
void GraphicsManagerClass::ParseAmbient(const char* info){ mAmbientLights.push_back(Color(Utils::GetVector(&info))); }
void GraphicsManagerClass::GetScreenshot(std::string name) { mImage.saveToFile(name); }
void GraphicsManagerClass::AddLight(const Light& light, const std::string& scene) { mLights[scene].push_back(light); }
void GraphicsManagerClass::IncrementSampleCount()
{
#ifdef MULTITHREAD
	if (ThreadPool.HasFinished()) {
		ThreadPool.Wait();
		mSampleCount++;
	}

#else
	mSampleCount++;
#endif // MULTITHREAD
}
glm::vec2 GraphicsManagerClass::GetNDC(const glm::vec2& xy)
{
	float x = ((xy.x + 0.5F) - (mWidth / 2.0F)) / (mWidth / 2.0F);
	float y = -((xy.y + 0.5F) - (mHeight / 2.0F)) / (mHeight / 2.0F);

	return glm::vec2(x, y);
}

glm::vec2 GraphicsManagerClass::GetRandomSubPixelNDC(const glm::vec2& xy)
{
	float x = ((xy.x + Utils::GetRandomFloat(0.0f, 1.0f)) - (mWidth / 2.0F)) / (mWidth / 2.0F);
	float y = -((xy.y + Utils::GetRandomFloat(0.0f, 1.0f)) - (mHeight / 2.0F)) / (mHeight / 2.0F);

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
		  + ndc.y * (mCameras[currScene].mUp / ((2.0F * mWidth) / mHeight));

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
std::vector<Light>& GraphicsManagerClass::GetLights(const std::string& key) { return mLights[key]; }
glm::ivec2 GraphicsManagerClass::GetSize() { return glm::ivec2(mWidth, mHeight); }
bool GraphicsManagerClass::RenderNormals(){ return mRenderNormals; }
int GraphicsManagerClass::GetTotalSamples() { return mSamples; }
int GraphicsManagerClass::GetSampleCount() { return mSampleCount; }
void GraphicsManagerClass::SetWidth(int width) { mWidth = width; }
void GraphicsManagerClass::SetRenderNormals(bool render) { mRenderNormals = render; }
void GraphicsManagerClass::ToggleRenderNormals() { mRenderNormals = !mRenderNormals; }
void GraphicsManagerClass::SetSamples(int count) { mSamples = count; }
void GraphicsManagerClass::SetAntiAliasingSamples(int count) { mMSAASamples = count; }
void GraphicsManagerClass::SetHeight(int height) { mHeight = height; }
void GraphicsManagerClass::SetAspectRatio(float ratio) { mAspectRatio = ratio;  }
void GraphicsManagerClass::SetAspectRatio() { mAspectRatio = (float)mWidth / mHeight;  }
bool GraphicsManagerClass::SwapBuffers() { return mSampleCount <= mSamples; }
#ifdef MULTITHREAD
#include "ThreadPool.h"
glm::ivec2 GraphicsManagerClass::GetBatchSize() { return mBatchSize; }

void GraphicsManagerClass::BatchedRender()
{
	if (!SwapBuffers()) return;
	if (!ThreadPool.HasFinished()) return;

	int xBatches = mWidth / mBatchSize.x;
	int yBatches = mHeight / mBatchSize.y;
	ThreadPool.SetTaskCount(xBatches * yBatches);

	//for each thread call render for the wanted coords
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
#endif // MULTITHREAD

