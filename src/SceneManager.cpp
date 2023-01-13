#include "SceneManager.h"
#include <filesystem>

void SceneManagerClass::LoadScenes(const char* _directory)
{
	for (auto& it : std::filesystem::directory_iterator(_directory))
		LoadScene(it.path().string());

}

void SceneManagerClass::LoadScene(const std::string& to_load)
{
	//load stuff
}
