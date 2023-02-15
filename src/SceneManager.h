#pragma once
#include <vector>

struct Scene;
class SceneManagerClass
{
public:

	void LoadScenes(const char* directory);
	void LoadScene(const std::string& to_load);
	void NextScene();
	void PrevScene();


	Scene* GetScene();
	int GetDisplayScene();

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

	int mDisplayScene = 2;
	std::vector<Scene*> mScenes;
};

#define SceneManager (SceneManagerClass::GetInstance())
