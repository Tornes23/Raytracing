#pragma once
#include <vector>
#include "Scene.h"

class SceneManagerClass
{
public:

	void LoadScenes(const char* directory);
	void LoadScene(const std::string& to_load);

	//singleton stuff
	SceneManagerClass(SceneManagerClass const&) = delete;
	void operator=(SceneManagerClass const&) = delete;
	static SceneManagerClass& GetInstance()
	{
		static SceneManagerClass instance;
		return instance;
	}

private:
	SceneManagerClass() {}

	std::vector<Scene> mScenes;
};

#define SceneManager (SceneManagerClass::GetInstance())
