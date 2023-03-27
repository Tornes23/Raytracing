#include <iostream>
#include <SFML/Graphics.hpp>
#include <glm/gtx/transform.hpp>

#include "FrameBuffer.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "Geometry.h"
#include "Raytracer.h"
#include "Scene.h"
#include "Utils.h"
#include "Window.h"
#ifdef MULTITHREAD
#include "ThreadPool.h"
#endif

int main(int argc, char ** argv)
{

    std::string inputDirectory= "./scenes/";
    //Loading the scene
    SceneManager.LoadScenes(inputDirectory.data());

    Utils::LoadConfig("CONFIG.txt");
    
    std::string inputFile = "input.txt";
    std::string screenshotName = "vergasos.png";
    bool        takeScreenshot = false;

    if (argc > 1)
        inputFile = argv[1];
    if (argc > 2)
    {
        screenshotName = argv[2];
        takeScreenshot = true;
    }
	
#ifdef MULTITHREAD
    ThreadPool.Init();
#endif // MULTITHREAD
    GraphicsManager.Init();

    glm::ivec2 size = GraphicsManager.GetSize();
    Window.Init(GraphicsManager.GetSize());

    bool reload = false;

    // Init the clock
    sf::Clock clock;
    while (Window.IsOpen())
    {
        // Handle input
        sf::Event event;
        while (Window.GetSFWindow().pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                Window.CloseWindow();
#ifdef MULTITHREAD
                ThreadPool.ShutDown();
#endif // MULTITHREAD
            }

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            Window.CloseWindow();
#ifdef MULTITHREAD
            ThreadPool.ShutDown();
#endif // MULTITHREAD
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
            takeScreenshot = true;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
            GraphicsManager.ToggleRenderNormals();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F6))
            reload = true;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
            SceneManager.NextScene();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F4))
            SceneManager.PrevScene();


        //update sample count here
        GraphicsManager.Update();
#ifdef MULTITHREAD
        //each thread will take care of the rendering of each batch
        GraphicsManager.BatchedRender();
#else
        GraphicsManager.Render();
#endif // MULTITHREAD

        Window.Update();
		
        GraphicsManager.UpdateTextures();
        Window.Draw(GraphicsManager.GetSprite());
        Window.Display();

        if (reload)
        {
            GraphicsManager.SetRenderNormals(false);
            GraphicsManager.Clear();
            reload = false;
        }
		
        if (takeScreenshot)
        {
            GraphicsManager.GetScreenshot(screenshotName);
            takeScreenshot = false;
        }
    }

#ifdef MULTITHREAD
    ThreadPool.ShutDown();
#endif // MULTITHREAD
    GraphicsManager.ShutDown();
    SceneManager.FreeScenes();

    return 0;
}