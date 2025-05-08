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
    mBuffer = new glm::vec3[size];
    Clear();
}

void FrameBuffer::Free()
{
    delete[] mBuffer;
}

void FrameBuffer::Clear(const glm::vec3& color)
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            SetPixel(x, y, color);
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
            mBuffer[(y * width + x)] = color;
        }
    }
}

void FrameBuffer::SetPixel(int x, int y, const glm::vec3& color)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
    {
        mBuffer[(y * width + x)] = color;
    }
}

void FrameBuffer::AddToPixel(int x, int y, const glm::vec3& color)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
        mBuffer[(y * width + x)] += color;
}

void FrameBuffer::SetPixel(int x, int y, const Color& color)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
        SetPixel(x, y, color.GetColor());
}

void FrameBuffer::AddToPixel(int x, int y, const Color& color)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
        AddToPixel(x, y, color.GetColor());
}

void FrameBuffer::Normalize(int startX, int startY, int endX, int endY, int factor)
{

    //DEBUG
    //std::cout << "normalizing batch = [" << startX << ", " << startY << ", " << width << ", " << height << "]\n";
    //std::cout << "normalizin factor = " << factor << "\n";
    //int x = 229;
    //int y = 196;
    for (int x = startX; x < endX; x++)
    {
        for (int y = startY; y < endY; y++)
        {
              mBuffer[(y * width + x)] /= factor;
        }
    }
}

Color FrameBuffer::GetColorFromPixel(int x, int y)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
    {
        return mBuffer[(y * width + x)];
    }

    return Color::Black;
}

void FrameBuffer::SwapBuffers()
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            SetPixel(x, y, mBuffer[(y * width + x)]);
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
            glm::vec<3, unsigned char> color = GetColorFromPixel(x, y).ToRGB(samples);
            image.setPixel(x, y, sf::Color(color.r, color.g, color.b));
        }
    }
}