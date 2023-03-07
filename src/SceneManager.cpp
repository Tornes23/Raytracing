#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <glm/common.hpp>

#include "SceneManager.h"
#include "GraphicsManager.h"
#include "Scene.h"
#include "Object.h"
#include "Utils.h"

void SceneManagerClass::LoadScenes(const char* _directory)
{
    for (auto& it : std::filesystem::directory_iterator(_directory))
        LoadScene(it.path().string());

}

void SceneManagerClass::LoadScene(const std::string& to_load)
{
    //stream to read from the file
    std::ifstream inFile(to_load, std::ios::in);
    char buffer[80] = { '\0' };
    //checking if it was opened
    if (!inFile)
        std::cerr << "ERROR WHILE TRYING TO OPEN " << to_load << " FILE\n";

    std::vector<Object> objects;
    //reading the code from the file and adding it to the string
    while (inFile.good())
    {
        inFile.getline(buffer, 80);
        if (!inFile.eof())
        {
            if (buffer[0] != '\0' && buffer[0] != '#')
            {
                std::string line(buffer);
                size_t found = line.find(' ');

                if (found != line.npos)
                {
                    std::string type = line.substr(0, found);

                    if (type.compare("CAMERA") == 0)
                        GraphicsManager.CreateCamera(line.substr(found + 1u).c_str());
                    else if (type.compare("AMBIENT") == 0)
                        GraphicsManager.ParseAmbient(line.substr(found + 1u).c_str());
                    else if (type.compare("LIGHT") == 0)
                        GraphicsManager.CreateLight(line.substr(found + 1u).c_str());
                    else
                    {
                        if (type.compare("SPHERE") == 0)
                            objects.push_back(Object(line.substr(found + 1u).c_str(), GeometryTypes::Sphere));
                        if (type.compare("BOX") == 0)
                            objects.push_back(Object(line.substr(found + 1u).c_str(), GeometryTypes::AABB));
                        if (type.compare("POLYGON") == 0)
                            objects.push_back(Object(line.substr(found + 1u).c_str(), GeometryTypes::Polygon));
                        if (type.compare("MESH") == 0)
                            objects.push_back(Object(line.substr(found + 1u).c_str(), GeometryTypes::Model));

                        objects.back().mMaterial = Utils::ParseMaterial(inFile);
                    }

                   
                }
            }
        }
    }
    
    for (auto obj : GraphicsManager.GetLights())
        objects.push_back(Object(obj));

    mScenes.push_back(new Scene(objects, Utils::GetFilename(to_load)));
    //closing the file
    inFile.close();
	//load stuff
}

void SceneManagerClass::PrevScene()
{
    mDisplayScene = glm::clamp(mDisplayScene - 1, 0, (int)mScenes.size());
}

void SceneManagerClass::NextScene()
{
    mDisplayScene = glm::clamp(mDisplayScene + 1, 0, (int)mScenes.size());
}

Scene* SceneManagerClass::GetScene()
{
    if (mDisplayScene < 0)
        return mScenes[0];
    if (mDisplayScene > mScenes.size())
        return mScenes.back();

    return mScenes[mDisplayScene];
}

int SceneManagerClass::GetDisplayScene(){ return mDisplayScene;}

void SceneManagerClass::SetDisplayScene(int index)
{
    if (mDisplayScene < 0)
        mDisplayScene = 0;
    if (mDisplayScene > mScenes.size())
        mDisplayScene = (int)mScenes.size();
    else
        mDisplayScene = index;
}

void SceneManagerClass::SetDisplayScene(const std::string& filename)
{
    for (int i = 0; i < mScenes.size(); i++) {
        if (mScenes[i]->mName == filename) {
            mDisplayScene = i;
            break;
        }
    }
}

void SceneManagerClass::FreeScenes()
{
    for (int i = 0; i < mScenes.size(); i++)
    {
        if(mScenes[i] != nullptr)
            delete mScenes[i];
        mScenes[i] = nullptr;
    }

    mScenes.clear();
}
