#include <string>
#include <glm/gtc/random.hpp>
#include <glm/gtc/constants.hpp>
#include "Utils.h"
#include "Material.h"
#include "GraphicsManager.h"
#include "SceneManager.h"

glm::vec3 Utils::GetVector(const char** info)
{
    if(info == nullptr)
        return glm::vec3(0.0F);

    glm::vec3 vec(0.0F);
    int coord = 0;

    while (**info != ')' && **info != '\0' && **info != '\n')
    {
        if (**info == '(' || **info == ',' || **info == ' ')
        {
            (*info)++;
            continue;
        }
        else
        {
            vec[coord] = GetFloat(info);
            coord++;
        }

    }

    (*info)++;
    return vec;
}

glm::vec3 Utils::GetVector(char** info)
{
    if (info == nullptr)
        return glm::vec3(0.0F);

    glm::vec3 vec(0.0F);
    int coord = 0;

    while (**info != ')' && **info != '\0' && **info != '\n')
    {
        if (**info == '(' || **info == ',' || **info == ' ')
        {
            (*info)++;
            continue;
        }
        else
        {
            vec[coord] = GetFloat(info);
            coord++;
        }

    }

    (*info)++;
    return vec;
}

float Utils::GetFloat(const char** info)
{
    if(info == nullptr)
        return 0.0f;

    float val = 0.0F;
    std::string number;
    char* end{};

    while (**info != '('  && **info != ',' && **info != ')' && **info != '\0' && **info != '\n')
    {
        if (**info == ' ')
        {
            (*info)++;
            continue;
        }

        number.push_back(**info);
        (*info)++;
    }

    val = std::strtof(number.data(), &end);

    return val;
}

float Utils::GetFloat(char** info)
{
    if (info == nullptr)
        return 0.0f;

    float val = 0.0F;
    std::string number;
    char* end{};

    while (**info != '(' && **info != ',' && **info != ')' && **info != '\0' && **info != '\n')
    {
        if (**info == ' ')
        {
            (*info)++;
            continue;
        }

        number.push_back(**info);
        (*info)++;
    }

    val = std::strtof(number.data(), &end);

    return val;
}

int Utils::GetInt(const char** info)
{
    if (info == nullptr)
        return -1;

    int val = 0;
    std::string number;
    char* end{};

    while (**info != '(' && **info != ',' && **info != ')' && **info != '\0' && **info != '\n')
    {
        if (**info == ' ')
        {
            (*info)++;
            continue;
        }

        number.push_back(**info);
        (*info)++;
    }

    val = std::atoi(number.c_str());

    return val;
}

int Utils::GetInt(char** info)
{
    if (info == nullptr)
        return -1;

    int val = 0;
    std::string number;
    char* end{};

    while (**info != '(' && **info != ',' && **info != ')' && **info != '\0' && **info != '\n')
    {
        if (**info == ' ')
        {
            (*info)++;
            continue;
        }

        number.push_back(**info);
        (*info)++;
    }

    val = std::atoi(number.c_str());

    return val;
}

int Utils::GetInt(const char* info)
{
    if (info == nullptr)
        return -1;

    int val = 0;
    std::string number;
    char* end{};

    while (*info != '(' && *info != ',' && *info != ')' && *info != '\0' && *info != '\n')
    {
        if (*info == ' ')
        {
            info++;
            continue;
        }

        number.push_back(*info);
        info++;
    }

    val = std::atoi(number.c_str());

    return val;
}

float Utils::GetFloat(const char* info)
{
	if (info == nullptr)
		return 0.0f;

	float val = 0.0F;
	std::string number;
	char* end{};

	while (*info != '(' && *info != ',' && *info != ')' && *info != '\0' && *info != '\n')
	{
		if (*info == ' ')
		{
			info++;
			continue;
		}

		number.push_back(*info);
		info++;
	}

	val = std::strtof(number.data(), &end);

	return val;
}

std::string Utils::GetFile(const char* info)
{
    if (info == nullptr)
        return std::string("info is null");

    return GetFilename(info);
}

Material* Utils::ParseMaterial(std::ifstream& file)
{
    Material* parsed = nullptr;
    char buffer[155] = { '\0' };
    do
    {
        file.getline(buffer, 155);
        if (buffer[0] == '\0' || buffer[0] == '#')
            break;

        //read diffuse
        std::string line(buffer);
        size_t found = line.find(' ');

        if (found != line.npos)
        {
            std::string type = line.substr(0, found);
            std::string vector = line.substr(found + 1u).data();
            const char* info = vector.data();
            if (type.compare("DIFFUSE") == 0){
                parsed = new Diffuse();
                parsed->ParseData(&info);
            }
            if (type.compare("METAL") == 0)
            {
                parsed = new Metal();
                parsed->ParseData(&info);
            }
            if (type.compare("DIELECTRIC") == 0)
            {
                parsed = new Dielectric();
                parsed->ParseData(&info);
            }
        }

    } while (!file.eof());

    return parsed;
}

std::string Utils::GetFilename(const std::string& path)
{
    if (path.empty())
        return path;

    std::string file = path.substr(path.find_last_of("/") + 1u);
    file = file.substr(0, file.find('.'));

    return file;
}

void Utils::LoadConfig(const std::string& path)
{
    //stream to read from the file
    std::ifstream inFile(path, std::ios::in);
    char buffer[80] = { '\0' };
    //checking if it was opened
    if (!inFile)
        std::cerr << "ERROR WHILE TRYING TO OPEN " << path << " FILE\n";

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

                    if (type.compare("WIDTH") == 0)
                        GraphicsManager.SetWidth(GetInt(line.substr(found + 1u).c_str()));
                    else if (type.compare("HEIGHT") == 0)
                        GraphicsManager.SetHeight(GetInt(line.substr(found + 1u).c_str()));
                    else if (type.compare("SAMPLES") == 0)
                        GraphicsManager.SetSamples(GetInt(line.substr(found + 1u).c_str()));
                    else if (type.compare("BOUNCES") == 0)
                        Raytracer.SetBounces(GetInt(line.substr(found + 1u).c_str()));
                    else if (type.compare("SCENE") == 0)
                        SceneManager.SetDisplayScene(GetFile(line.substr(found + 1u).c_str()));
                    else if (type.compare("NORMALS") == 0)
                        GraphicsManager.SetRenderNormals(GetInt(line.substr(found + 1u).c_str()) == 1 ? true : false );
					else if (type.compare("ANTIALIASING_SAMPLES") == 0)
                        GraphicsManager.SetAntiAliasingSamples(GetInt(line.substr(found + 1u).c_str()));
					else if (type.compare("KDTREE_TRAVERSAL_COST") == 0)
                        SceneManager.GetCurrentScene()->SetKDTreeTraversalCost(GetFloat(line.substr(found + 1u).c_str()));
					else if (type.compare("KDTREE_INTERSECTION_COST") == 0)
						SceneManager.GetCurrentScene()->SetKDTreeIntersectionCost(GetFloat(line.substr(found + 1u).c_str()));
					else if (type.compare("KDTREE_MAX_DEPTH") == 0)
						SceneManager.GetCurrentScene()->SetKDTreeMaxDepth(GetInt(line.substr(found + 1u).c_str()));
                }
            }
        }
    }
    //closing the file
    inFile.close();
}

glm::vec3 Utils::GetRandomVector() { return  glm::sphericalRand(1.0F); }
glm::vec3 Utils::GetRandomVector(float radius) { return  glm::ballRand(radius); }

float Utils::GetRandomFloat(float min, float max)
{
    return glm::linearRand(min, max);
}
