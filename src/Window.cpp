#include <regex>
#include "GraphicsManager.h"
#include "Window.h"

void WindowClass::Init(const glm::ivec2& size, const std::string& title)
{
	mTitle = title;
	mWindow.create(sf::VideoMode(size.x, size.y), mTitle.data());
	mbOpen = true;
}

void WindowClass::Update()
{
	std::string title = std::regex_replace(mTitle, std::regex("%c"), std::to_string(GraphicsManager.GetSampleCount()));
	title = std::regex_replace(title, std::regex("%s"), std::to_string(GraphicsManager.GetTotalSamples()));
	title = std::regex_replace(title, std::regex("%f"), GraphicsManager.SwapBuffers() ? "false" : "true");
	mWindow.setTitle(title.data());
}

bool WindowClass::IsOpen(){	return mbOpen; }

void WindowClass::CloseWindow() { mbOpen = false; mWindow.close(); }

sf::RenderWindow& WindowClass::GetSFWindow() { return mWindow; }

void WindowClass::Draw(const sf::Sprite& sprite){ mWindow.draw(sprite); }

void WindowClass::Display() { mWindow.display(); }

