#include <string>
#include <glm/gtc/random.hpp>
#include <glm/gtc/constants.hpp>
#include "Utils.h"
#include "Material.h"

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

std::string Utils::GetFile(const char** info)
{
    return std::string();
}

Material* Utils::ParseMaterial(std::ifstream& file)
{
    Material* parsed = nullptr;
    char buffer[40] = { '\0' };
    do
    {
        file.getline(buffer, 40);
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
            if (type.compare("METAl") == 0)
            {
                parsed = new Diffuse();
                parsed->ParseData(&info);
            }
            if (type.compare("DIELECTRIC") == 0)
            {
                parsed = new Diffuse();
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
    //load the configurations
}

glm::vec3 Utils::GetRandomVector() { return  glm::sphericalRand(1.0F); }
