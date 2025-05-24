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
	void BuildScenesKDTrees();


	Scene* GetScene(int i = -1);
	Scene* GetCurrentScene();
	int GetDisplayScene();
	void SetDisplayScene(int index = 0);
	void SetDisplayScene(const std::string& filename);

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

	int mDisplayScene = -1;
	std::vector<Scene*> mScenes;
};

#define SceneManager (SceneManagerClass::GetInstance())
