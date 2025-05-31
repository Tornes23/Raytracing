#pragma once
#include <SFML/Graphics.hpp>
#include <glm/vec3.hpp>
#include "Color.h"

class FrameBuffer
{
  public:
    void Init(int w, int h);
    void Free();

    void Clear(const glm::vec3& color = glm::vec3(0.0F));
    void ClearBatch(int startX, int startY, int endX, int endY, const glm::vec3& color = glm::vec3(0.0F));

    void SetRaytracingPixel(int x, int y, const glm::vec3& color);
    void AddToRaytracingPixel(int x, int y, const glm::vec3& color);
    void SetRaytracingPixel(int x, int y, const Color& color);
    void AddToRaytracingPixel(int x, int y, const Color& color);

    Color GetColorFromRaytracingPixel(int x, int y);
    glm::vec<3, unsigned char> GetColorFromRenderPixel(int x, int y);
    void SetColorRenderPixel(int x, int y, const glm::vec<3, unsigned char>& color);

    void ConvertRaytracingPixelToRenderPixel(int x, int y, int divisor = 1);

    void SwapBuffers();

    int  GetWidth() { return width; }
    int  GetHeight() { return height; }

    void ConvertFrameBufferToSFMLImage(sf::Image & image, int samples = 1);

  private:
    int                         width;
    int                         height;
    glm::vec3*                  mRaytracingBuffer;
    glm::vec<3, unsigned char>* mRenderBuffer;
};
