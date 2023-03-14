#pragma once
#include <glm/vec2.hpp>
#include <SFML/Graphics.hpp>

class WindowClass
{
public:

	void Init(const glm::ivec2& size, const std::string& title = "Raytracer | Samples: %c/%s Finished: %f");
	void Update();
	bool IsOpen();
	void CloseWindow();
	sf::RenderWindow& GetSFWindow();
	void Draw(const sf::Sprite& sprite);
	void Display();
	void Update(float dt);

	//singleton stuff
	WindowClass(WindowClass const&) = delete;
	void operator=(WindowClass const&) = delete;
	static WindowClass& GetInstance()
	{
		static WindowClass instance;
		return instance;
	}
private:
	WindowClass() {}
	std::string mTitle;
	sf::RenderWindow mWindow;
	bool mbOpen = false;
};

#define Window (WindowClass::GetInstance())
