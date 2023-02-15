#include <string>
#include <glm/gtc/random.hpp>
#include <glm/gtc/constants.hpp>
#include "Utils.h"

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

std::string Utils::GetFile(const char** info)
{
    return std::string();
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

glm::vec3 Utils::GetRandomVector()
{
    float theta = glm::linearRand(0.0f, 1.0f) * 2.0 * glm::pi<float>();
    float phi = glm::acos(glm::linearRand(-1.0f, 1.0f));
    float x = sin(phi) * cos(theta);
    float y = sin(phi) * sin(theta);
    float z = cos(phi);

    return glm::vec3(x, y, z);
}
