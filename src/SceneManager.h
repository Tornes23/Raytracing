#pragma once
#include <vector>

class Scene;
class SceneManagerClass
{
public:

	void LoadScenes(const char* directory);
	void LoadScene(const std::string& to_load);

	void FreeScenes();
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

	std::vector<Scene*> mScenes;
};

#define SceneManager (SceneManagerClass::GetInstance())
