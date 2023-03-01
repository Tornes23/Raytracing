#pragma once
#include <SFML/Graphics.hpp>

class FrameBuffer
{
  public:
    void Init(int w, int h);
    void Free();

    void Clear(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0);
    void ClearBatch(int startX, int startY, int endX, int endY, unsigned char r = 0, unsigned char g = 0, unsigned char b = 0);
    void SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    void AddToPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    void Normalize(int startX, int startY, int endX, int endY, int factor);
    void GetPixel(int x, int y, unsigned char & r, unsigned char & g, unsigned char & b);
    int  GetWidth() { return width; }
    int  GetHeight() { return height; }

    void ConvertFrameBufferToSFMLImage(sf::Image & image);

  private:
    int             width;
    int             height;
    unsigned char * imageData;
};
