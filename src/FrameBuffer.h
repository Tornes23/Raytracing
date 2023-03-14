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

    void SetPixel(int x, int y, const glm::vec3& color);
    void AddToPixel(int x, int y, const glm::vec3& color);
    void SetPixel(int x, int y, const Color& color);
    void AddToPixel(int x, int y, const Color& color);

    void Normalize(int startX, int startY, int endX, int endY, int factor);
    Color GetPixel(int x, int y);
    void SwapBuffers();

    int  GetWidth() { return width; }
    int  GetHeight() { return height; }

    void ConvertFrameBufferToSFMLImage(sf::Image & image, int samples = 1);

  private:
    int             width;
    int             height;
    glm::vec3 *     mBuffer;
};
