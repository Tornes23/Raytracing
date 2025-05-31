#include <iostream>
#include <glm/glm.hpp>
#include "GraphicsManager.h"
#include "FrameBuffer.h"
#ifdef MULTITHREAD
#include <mutex>
#include <thread>
#endif // MULTITHREAD


void FrameBuffer::Init(int w, int h)
{
    width     = w;
    height    = h;
    int size  = width * height;
    mRaytracingBuffer = new glm::vec3[size];
    mRenderBuffer = new glm::vec<3, unsigned char>[size];
    Clear();
}

void FrameBuffer::Free()
{
    delete[] mRaytracingBuffer;
    delete[] mRenderBuffer;
}

void FrameBuffer::Clear(const glm::vec3& color)
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            SetRaytracingPixel(x, y, color);
        }
    }
}

void FrameBuffer::ClearBatch(int startX, int startY, int endX, int endY, const glm::vec3& color)
{
    //DEBUG
    //std::cout << "clearing batch = [" << startX << ", " << startY << ", " << endX << ", " << endY << "]\n";

    for (int x = startX; x < endX; x++)
    {
        for (int y = startY; y < endY; y++)
        {
            mRaytracingBuffer[(y * width + x)] = color;
        }
    }
}

void FrameBuffer::SetRaytracingPixel(int x, int y, const glm::vec3& color)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
    {
        mRaytracingBuffer[(y * width + x)] = color;
    }
}

void FrameBuffer::AddToRaytracingPixel(int x, int y, const glm::vec3& color)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
    {
        mRaytracingBuffer[(y * width + x)] += color;
    }
}

void FrameBuffer::SetRaytracingPixel(int x, int y, const Color& color)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
        SetRaytracingPixel(x, y, color.GetColor());
}

void FrameBuffer::AddToRaytracingPixel(int x, int y, const Color& color)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
        AddToRaytracingPixel(x, y, color.GetColor());
}

Color FrameBuffer::GetColorFromRaytracingPixel(int x, int y)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
    {
        return mRaytracingBuffer[(y * width + x)];
    }

    return Color::Black;
}

glm::vec<3, unsigned char> FrameBuffer::GetColorFromRenderPixel(int x, int y)
{
	if (x >= 0 && y >= 0 && x < width && y < height)
	{
		return mRenderBuffer[(y * width + x)];
	}

	return glm::vec<3, unsigned char>(0,0,0);
}

void FrameBuffer::SetColorRenderPixel(int x, int y, const glm::vec<3, unsigned char>& color)
{
	if (x >= 0 && y >= 0 && x < width && y < height)
	{
		mRenderBuffer[(y * width + x)] = color;
	}
}

void FrameBuffer::ConvertRaytracingPixelToRenderPixel(int x, int y, int divisor)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
    {
        int index = (y * width + x);
        glm::vec3 pixelColor = (255.99f * mRaytracingBuffer[index]) / static_cast<float>(divisor);
        unsigned char red   = static_cast<unsigned char>(glm::clamp(pixelColor.r, 0.0f, 255.9f));
        unsigned char green = static_cast<unsigned char>(glm::clamp(pixelColor.g, 0.0f, 255.9f));
        unsigned char blue  = static_cast<unsigned char>(glm::clamp(pixelColor.b, 0.0f, 255.9f));
        glm::vec<3, unsigned char> renderColor(red, green, blue);

        SetColorRenderPixel(x, y, glm::vec<3, unsigned char>(red, green, blue));
    }
}

void FrameBuffer::SwapBuffers()
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            SetRaytracingPixel(x, y, mRaytracingBuffer[(y * width + x)]);
        }
    }
}

// Convert the custom framebuffer to SFML format
void FrameBuffer::ConvertFrameBufferToSFMLImage(sf::Image & image, int samples)
{
    //if (!GraphicsManager.SwapBuffers()) samples--;

    int w = FrameBuffer::GetWidth();
    int h = FrameBuffer::GetHeight();
    //int x = 229;
    //int y = 196;
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            glm::vec<3, unsigned char> color = GetColorFromRenderPixel(x, y);
            image.setPixel(x, y, sf::Color(color.r, color.g, color.b));
        }
    }
}