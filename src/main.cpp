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
    GraphicsManager.Init(WIDTH, HEIGHT);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");

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

#ifdef MULTITHREAD

        //each thread will take care of the rendering of each batch
        GraphicsManager.BatchedRender();
        
#else
        GraphicsManager.Render();
#endif // MULTITHREAD

        // Fill framebuffer
        sf::Time elapsed = clock.getElapsedTime();
        int      time    = static_cast<int>(elapsed.asSeconds());

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
            takeScreenshot = true;
		
        // Show image on screen
        FrameBuffer::ConvertFrameBufferToSFMLImage(GraphicsManager.GetImage());

        GraphicsManager.Update();
        window.draw(GraphicsManager.GetSprite());
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
            GraphicsManager.GetImage().saveToFile(screenshotName);
            takeScreenshot = false;
        }
    }

    return 0;
}