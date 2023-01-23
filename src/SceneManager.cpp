#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "SceneManager.h"
#include "GraphicsManager.h"
#include "Object.h"

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
                        GraphicsManager.GetAmbient(line.substr(found + 1u).c_str());
                    else
                    {
                        if (type.compare("SPHERE") == 0)
                            objects.push_back(Object(line.substr(found + 1u).c_str(), GeometryTypes::Sphere));
                        if (type.compare("BOX") == 0)
                            objects.push_back(Object(line.substr(found + 1u).c_str(), GeometryTypes::AABB));

                        objects.back().mMaterial.ParseData(inFile);
                    }

                   
                }
            }
        }
    }

    mScenes.push_back(Scene(objects));
    //closing the file
    inFile.close();
	//load stuff
}
