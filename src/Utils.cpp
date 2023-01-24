#include <string>
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

    while (**info != ',' && **info != ')' && **info != '\0' && **info != '\n')
    {
        number.push_back(**info);
        (*info)++;
    }

    val = std::strtof(number.data(), &end);

    return val;
}

std::string Utils::GetFilename(const std::string& path)
{
    if (path.empty())
        return path;

    std::string file = path.substr(path.find_last_of("/") + 1u);
    file = file.substr(0, file.find('.'));

    return file;
}
