#include <iostream>
#include <SFML/Graphics.hpp>
#include <glm/gtx/transform.hpp>

#include "FrameBuffer.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "Geometry.h"
#include "Raytracer.h"
#include "Scene.h"

int main(int argc, char ** argv)
{
    const int WIDTH  = 500;
    const int HEIGHT = 500;
    GraphicsManager.SetWidth(WIDTH);
    GraphicsManager.SetHeight(HEIGHT);
    GraphicsManager.SetAspectRatio((float)WIDTH / HEIGHT);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");

    FrameBuffer::Init(WIDTH, HEIGHT);

    std::string inputDirectory= "./scenes/";
    std::string inputFile = "input.txt";
    std::string screenshotName = "screenshot.png";
    bool        takeScreenshot = false;

    if (argc > 1)
        inputFile = argv[1];
    if (argc > 2)
    {
        screenshotName = argv[2];
        takeScreenshot = true;
    }
	
    //Loading the scene
    SceneManager.LoadScenes(inputDirectory.data());
    // Generate image and texture to display
    sf::Image   image;
    sf::Texture texture;
    sf::Sprite  sprite;
    texture.create(WIDTH, HEIGHT);
    image.create(WIDTH, HEIGHT, sf::Color::Black);

    bool reload = false;

    // Init the clock
    sf::Clock clock;
    while (window.isOpen())
    {
        // Handle input
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
            GraphicsManager.ToggleRenderNormals();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F4))
            reload = true;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
            SceneManager.NextScene();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
            SceneManager.PrevScene();

        //apply lighting based on the ray hits
        for (unsigned x = 0; x < WIDTH; x++)
        {
            for (unsigned y = 0; y < HEIGHT; y++)
            {
                glm::vec2 ndc = GraphicsManager.GetNDC({x,y});
                glm::vec3 pixelworld = GraphicsManager.GetPixelWorld(ndc);
                glm::vec3 camPos = GraphicsManager.GetCameraPos();
                Ray ray(camPos, glm::normalize(pixelworld - camPos));
                Scene* scene = SceneManager.GetScene();

                ContactInfo info = Raytracer.CastRay(ray, scene->mObjects);
                if (info.mTI >= 0.0f)
                {
                    if (GraphicsManager.RenderNormals())
                    {
                        Color result(Color((info.mNormal + glm::vec3(1.0F)) / 2.0F) * GraphicsManager.GetAmbient(SceneManager.GetDisplayScene()));
                        FrameBuffer::SetPixel(x, y, result.mR, result.mG, result.mB);
                    }
                    else
                    {
                        Color result = info.mColor * GraphicsManager.GetAmbient();
                        FrameBuffer::SetPixel(x, y, result.mR, result.mG, result.mB);
                    }
                }
            }
        }

        // Fill framebuffer
        sf::Time elapsed = clock.getElapsedTime();
        int      time    = static_cast<int>(elapsed.asSeconds());

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
            takeScreenshot = true;
		
        // Show image on screen
        FrameBuffer::ConvertFrameBufferToSFMLImage(image);

        texture.update(image);
        sprite.setTexture(texture);
        window.draw(sprite);
        window.display();
		
        if (reload)
        {
            SceneManager.FreeScenes();
            SceneManager.LoadScenes(inputDirectory.data());
            GraphicsManager.SetRenderNormals(false);
            reload = false;
        }
		
        if (takeScreenshot)
        {
            image.saveToFile(screenshotName);
            takeScreenshot = false;
        }
    }

    return 0;
}