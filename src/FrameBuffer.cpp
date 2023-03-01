#include <iostream>
#include <glm/glm.hpp>
#include "FrameBuffer.h"
#ifdef MULTITHREAD
#include <mutex>
#include <thread>
#endif // MULTITHREAD


void FrameBuffer::Init(int w, int h)
{
    width     = w;
    height    = h;
    int size  = 3 * width * height;
    imageData = new unsigned char[size];
    Clear(0, 0, 0);
}

void FrameBuffer::Free()
{
    delete[] imageData;
}

void FrameBuffer::Clear(unsigned char r, unsigned char g, unsigned char b)
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            imageData[(y * width + x) * 3 + 0] = r;
            imageData[(y * width + x) * 3 + 1] = g;
            imageData[(y * width + x) * 3 + 2] = b;
        }
    }
}

void FrameBuffer::ClearBatch(int startX, int startY, int endX, int endY, unsigned char r, unsigned char g, unsigned char b)
{
    //DEBUG
    //std::cout << "clearing batch = [" << startX << ", " << startY << ", " << endX << ", " << endY << "]\n";

    for (int x = startX; x < endX; x++)
    {
        for (int y = startY; y < endX; y++)
        {
            imageData[(y * width + x) * 3 + 0] = r;
            imageData[(y * width + x) * 3 + 1] = g;
            imageData[(y * width + x) * 3 + 2] = b;
        }
    }
}

void FrameBuffer::SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
    {
        imageData[(y * width + x) * 3 + 0] = r;
        imageData[(y * width + x) * 3 + 1] = g;
        imageData[(y * width + x) * 3 + 2] = b;
    }
}

void FrameBuffer::AddToPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
    {

        imageData[(y * width + x) * 3 + 0] = (unsigned char)glm::clamp<int>(imageData[(y * width + x) * 3 + 0] + r, 0, 255);
        imageData[(y * width + x) * 3 + 1] = (unsigned char)glm::clamp<int>(imageData[(y * width + x) * 3 + 1] + g, 0, 255);
        imageData[(y * width + x) * 3 + 2] = (unsigned char)glm::clamp<int>(imageData[(y * width + x) * 3 + 2] + b, 0, 255);
    }
}

void FrameBuffer::Normalize(int startX, int startY, int endX, int endY, int factor)
{

    //DEBUG
    //std::cout << "normalizing batch = [" << startX << ", " << startY << ", " << width << ", " << height << "]\n";

    for (int x = startX; x < endX; x++)
    {
        for (int y = startY; y < endY; y++)
        {
            imageData[(y * width + x) * 3 + 0] /= (unsigned char)factor;
            imageData[(y * width + x) * 3 + 1] /= (unsigned char)factor;
            imageData[(y * width + x) * 3 + 2] /= (unsigned char)factor;
        }
    }
}

void FrameBuffer::GetPixel(int x, int y, unsigned char & r, unsigned char & g, unsigned char & b)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
    {
        r = imageData[(y * width + x) * 3 + 0];
        g = imageData[(y * width + x) * 3 + 1];
        b = imageData[(y * width + x) * 3 + 2];
    }
}

// Convert the custom framebuffer to SFML format
void FrameBuffer::ConvertFrameBufferToSFMLImage(sf::Image & image)
{
    int w = FrameBuffer::GetWidth();
    int h = FrameBuffer::GetHeight();

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            unsigned char r, b, g;
            FrameBuffer::GetPixel(x, y, r, g, b);
            image.setPixel(x, y, sf::Color(r, g, b));
        }
    }
}